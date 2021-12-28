#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define MAXLINE (10000)     /* maximum line length is 10,000 */

int _pass_count;
_pass_count = 0;

void pass()
{
    printf("\tpass: %d\n", _pass_count);
    _pass_count++;
}




typedef struct _context {
    wchar_t *key;
    wchar_t *substitute;
    struct _context *next;
} context;

int compile(wchar_t *tf, wchar_t *of, wchar_t **s);
context *parse_str(wchar_t **s);
int compile_template(FILE *template, FILE *output, context *t, int vflag);
int getlinef(FILE *fp, wchar_t *s, int lim);
context *s_all_contexts(wchar_t *s, context *t);
wchar_t *s_immediate_context(wchar_t *s, context *t);
void print_context(context *t, int recursive, int is_sub);
wchar_t *wcsstrip(wchar_t *s);

int main(int argc, char **argv)
{

    if (argc < 2) {
        printf("too few argument");
        return;
    }
    wchar_t *s[] = {
                    L"huhu", L"s:resun",
                    L"{{ post_content }}", L"f: C:/Users/_resun/Desktop/todo.txt",
                    L"too", L"g:to",
                    L"hey", L"s:hu",
                    L"NULL"};

    wchar_t *s1, *s2;
    char *a1, *a2;
    size_t s1_l, s2_l, s1_s, s2_s;

    a1 = *++argv;
    a2 = *++argv;

    s1_s = (size_t)(sizeof(wchar_t *) * (strlen(a1) + 1));
    s2_s = (size_t)(sizeof(wchar_t *) * (strlen(a2) + 1));

    s1 = (wchar_t *) malloc(s1_s);
    s2 = (wchar_t *) malloc(s2_s);

    mbstowcs(s1, a1, s1_s);
    mbstowcs(s2, a2, s2_s);

    printf("%d\n", compile(s1, s2, s));
}

int compile (wchar_t *tf, wchar_t *of, wchar_t **s)
{
    int r;
    FILE *template, *output;
    context *t;
    r = 0;
    template = _wfopen (tf, L"r");
    output = _wfopen (of, L"w");

    t = parse_str(s);
    //print_context(t, 1, 0);
    r = compile_template(template, output, t, 0);
    
    fclose(template);
    fclose(output);

    return r;
}



context *parse_str(wchar_t **s)
{
    context *t;

    wchar_t *temp1, *s1, temp[1000];
    int i;

    t = (context *) malloc (sizeof(context));

    t->key = t->substitute = NULL;
    t->next = NULL;

    t->key = wcslen(*s) > 0 ?  *s: NULL;
    s++;

    t->substitute = wcslen(*s) > 0 ?  *s: NULL;
    s++;

    if (wcscmp(*s, L"NULL") != 0) {
        t->next = parse_str(s);
    } else {
        t->next = NULL;
        return t;
    }
    return t;
}

/* compile the template */
int compile_template(FILE *template, FILE *output, context *t, int vflag)
{
    int count;
    wchar_t *line, *line1, *s, *s1;
    size_t line_len;
    context *t1;

    count = 0;
    t1 = NULL;
    line_len = (size_t)(sizeof(wchar_t *) * MAXLINE);
    line = (wchar_t *) malloc(line_len);
    line1 = (wchar_t *) malloc(line_len);

    if (line == NULL || line1 == NULL) {
        printf("!Could not allocate memory\n");
        exit(0);
    }

compile_line:
    while (getlinef(template, line, MAXLINE) > 0) {
        if ((t1 = s_all_contexts(line, t)) == NULL) {
            fwprintf(output, L"%ls", line);
            goto compile_line;
        } else {
            while(t1 != NULL) {
                s1 = wcsstr(line, t1->key);
                *s1 = L'\0';
                fwprintf(output, L"%ls", line);     /* write everything before the key string */
                line = s1 + wcslen(t1->key);
replacement:
                s = &(t1->substitute[2]);           /* s point to the value of the substitute */
                switch(t1->substitute[0]) {         /* resolve the the substitute string */
                    case 's':
                        goto substitute_string;
                    case 'f':
                        goto substitute_file;
                    default:
                        goto substitution_err;
                }
substitute_file:
                s = wcsstrip(s);
                FILE *fp = _wfopen(s, L"r, ccs=UTF-8");
                if (fp == NULL) {
                    wprintf(L"file: %s is NULL\n", s);
                    goto do_next;
                }
                while (getlinef(fp, line1, MAXLINE) > 0) {
                    fwprintf(output, L"%ls", line1);
                }
                fclose(fp);
                count++;
                if (vflag)
                    wprintf(L"replaced \"%s\" with the contents of:\tfile: %s\n", t1->key, s);
                goto do_next;
substitute_string:
                fwprintf(output, L"%ls", s);
                count++;
                if (vflag)
                    wprintf(L"replaced \"%s\" with:\t\"%s\"\n", t1->key, s);
                goto do_next;
substitution_err:
                wprintf(L"Could not resolve context:\n");
                print_context(t1, 0, 1);
do_next:
                t1 = t1->next;
            }
            fwprintf(output, L"%ls", line);         /* write everything after the key string */
        }
        free(t1);
    }
    return count;
}

/* readline in wchar_t *s from fp*/
int getlinef(FILE *fp, wchar_t *s, int lim)
{
    int i;
    wint_t c;

    i = 0;

    while (--lim > 0 && (c = getwc(fp)) != WEOF && c != L'\n') {
        *s++ = c;
        i++;
    }
    if (c == L'\n') {
        *s++ = c;
        i++;
    }
    *s = L'\0';
    return i;
}

context *temp_context;          /* context to share between s_all_contexts and s_immediate_context */

/* return all the found context in line s*/
context *s_all_contexts(wchar_t *s, context *t)
{
    wchar_t *s1;
    context *r, *r1, *r2;

    r = r1 = r2 = NULL;

    while (*s) {
        if ((s1 = s_immediate_context(s, t)) != NULL) {
            s = s1;
            r = (context *) malloc(sizeof(context));
            if (r1 == NULL) {
                r1 = r2 = r;
            } else if (r2->next == NULL) {
                r2->next = r;
                r2 = r2->next;
            }
            r->key = temp_context->key;
            r->substitute = temp_context->substitute;
            r->next = NULL;
        }
        if (r1 == NULL) {
            break;
        } else if (s1 == NULL) {
            break;
        }
    }
    return r1;
}

/* return the context that was found immediately in s */
wchar_t *s_immediate_context(wchar_t *s, context *t)
{
    wchar_t *s1, *s2;
    context *r;

    s1 = s2 = NULL;
    temp_context = NULL;
    while (t != NULL) {
        if ((s1 = wcsstr(s, t->key)) != NULL) {
            s1 += wcslen(t->key);
            if (s2 == NULL) {
                s2 = s1;
                temp_context = t;
            } else if (s2 > s1) {
                s2 = s1;
                temp_context = t;
            }
        }
        t = t->next;
    }
    return s2;
}

/* print a context */
void print_context(context *t, int recursive, int is_sub)
{
    int is_sub_t;

    is_sub_t = is_sub;
    if (t != NULL) {
        while (is_sub_t--)
            printf("\t");
        is_sub_t = is_sub;
        wprintf (L"key:\t%ls\n", t->key);
        while (is_sub_t--)
            printf("\t");
        is_sub_t = is_sub;
        wprintf (L"substitute:\t%ls\n", t->substitute);
        if (recursive > 0) {
            if (t->next != NULL) {
                while (is_sub_t--)
                    printf("\t");
                wprintf (L"next:\n");
                print_context (t->next, 1, (is_sub+1));
            } else {
                while (is_sub_t--)
                    printf("\t");
                printf("next:\tNULL\n\n\n");
            }
        } else {
            printf("\n\n");
        }
    } else {
        printf("context is Null\n");
    }
}

/* strip a wide char array */
wchar_t *wcsstrip(wchar_t *s)
{
    wchar_t *s1;

strip_front:
    while(*s) {
        if (!iswspace(*s)) {
            s1 = s;
            goto strip_end;
        }
        s++;
    }

strip_end:
    while(*s)
        s++;
    while(*--s) {
        if (iswspace(*s)) {
            *s = L'\0';
        } else {
            return s1;
        }
    }

    return NULL;
}