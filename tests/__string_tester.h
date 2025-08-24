#define STRING_INIT_MEM_AFTER_ALLOC
#define STRING_IMPLEMENTATION
#include <stdio.h>
#include "../String.h"
#include "ansi_colors.h"
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <execinfo.h>

extern bool is_atty;
extern const int n_total;
extern int fn_status[];
extern int tester_header_length;

#define DEFAULT_TESTER_HEADER_LENGTH 50
#define HGREEN_TXT_NR(_txt) __MAKE_TEXT_COLOR_BASE_NO_RESET(GREEN_HIGH_TEXT, _txt)
#define HRED_TXT_NR(_txt)   __MAKE_TEXT_COLOR_BASE_NO_RESET(RED_HIGH_TEXT, _txt)

typedef enum {
    FNS_UNKNOWN_BEAHAVIOR = -1,
    FNS_FUNCTION_SUCCED,
    FNS_FUNCTION_FAILED,
}FUNCTION_STATUS;

static int __print_tail_result(int n_tot){
    int __r = 0;

    /* getting the info */
    int n_succed = 0;
    for(int i = 0; i < n_total; ++i){
        if(fn_status[i] == FNS_FUNCTION_SUCCED) n_succed++;
    }

    /* printing it */
    if(is_atty){
#if 0
        __r = fwprintf(stdout, L"║\n╚═[");  /* start of results */
        __r += fwprintf(stdout, L""MK_HGREEN_TXT("%d")"/", n_succed);
        __r += fwprintf(stdout, L""MK_HCYAN_TXT("%d") " ", n_tot);
        __r += fwprintf(stdout, L""MK_HRED_TXT("%d")"", (n_tot - n_succed));
        __r += fwprintf(stdout, L"]", n_tot);
#endif
#if 0
        fwprintf(stdout, L"║\n");
#endif
        __r = fwprintf(stdout, L"╚═[");
        __r += fwprintf(stdout, L"Succed: "MK_HGREEN_TXT("%d")", ", n_succed);
        __r += fwprintf(stdout, L"Failed: "MK_HRED_TXT("%d")", ", (n_tot - n_succed));
        __r += fwprintf(stdout, L"Tot: "MK_HCYAN_TXT("%d"), n_tot);
        __r += fwprintf(stdout, L"]", n_tot);

        __r -= ((3 * (sizeof(MK_HGREEN_TXT("")) - 1))); /* we remove the ansi escape sequence */
    }
    else /* !is_atty */
#if 0
    __r = fwprintf(stdout, L"╚═[V:%d F:%d T:%d]", n_succed, (n_tot - n_succed), n_tot);
#else
    __r = fwprintf(stdout, L"╚═[Succed:%d, Failed:%d, Tot:%d]", n_succed, (n_tot - n_succed), n_tot);
#endif    
    
    return __r;
}

// You need to pass the -rdynamic to the compiler:
// https://stackoverflow.com/questions/6934659/how-to-make-backtrace-backtrace-symbols-print-the-function-names
// https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
// Something strange happen with this function
// static void __impl_signal_handler(int sig){
//     if(sig == SIGSEGV){
//         void *array[10];
//         char **strings;
//         int size, i;
//         size = backtrace(array, 10);
//         strings = backtrace_symbols(array, size);
//         if(strings != NULL){
//             for (i = 0; i < size; i++) 
//                 printf("%s\n", strings[i]);
//         }
//         free(strings);
//     }
//     exit(sig);
// }

static void __impl_test_start(const char* file){
    // signal(SIGSEGV, __impl_signal_handler);
    /* init */
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
    is_atty = isatty(STDOUT_FILENO);
    
    if(tester_header_length <= 0){
        struct winsize s = {0};
        if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &s) < 0){
            perror("error: ");
            exit(1);
        }
        tester_header_length = (s.ws_col / 2) /* Half the terminal */;
        // fwprintf(stdout, L"Term size: {w: %d, h: %d}\n", s.ws_col, s.ws_row);
    }

    /* tesetr info graphic header */
    wchar_t header[500] = {0};
    size_t hd_len = 0;
    size_t addlen = 0;
    // TIG = Test Info graphic
    if(is_atty){
        addlen = sizeof(MK_HMAGENTA_TXT("")) - 1;
        hd_len = swprintf(header, sizeof(header) / sizeof(wchar_t), L"╔═TIG═["MK_HMAGENTA_TXT("%s")"]", file);
    }
    else 
        hd_len = swprintf(header, sizeof(header) / sizeof(wchar_t), L"╔═TIG═[%s]", file); 
    
    for(size_t i = hd_len; i < (tester_header_length + addlen); ++i){
        header[i] = L'═';
    }
    fwprintf(stdout, L"%ls╗\n", header);
}

static void __impl_test_end(void){
    int i = fwprintf(stdout, L"");
    i += __print_tail_result(n_total);

    for(; i < tester_header_length; ++i){
        fputwc(L'═', stdout);
    }
    fwprintf(stdout, L"╝\n");
}

// This prints a single tester function info
void __impl_tester_log(CStr fname, size_t idx, CStr res, CStr expected){
   int f_status = fn_status[idx];
    if(is_atty){
        int bytes = 0;
        bytes += fwprintf(stdout, L"║ [%zu] ", idx);
        bytes += fwprintf(stdout, L"Function: ");
        if(f_status == FNS_UNKNOWN_BEAHAVIOR)
            fwprintf(stdout, L""MK_HYELLOW_TXT("%-*s"), tester_header_length - bytes, fname);
        else if(f_status == FNS_FUNCTION_SUCCED)
            fwprintf(stdout, L""MK_HGREEN_TXT("%-*s"), tester_header_length - bytes, fname);
        else if(f_status == FNS_FUNCTION_FAILED)
            fwprintf(stdout, L""MK_HRED_TXT("%-*s"), tester_header_length - bytes, fname);
        fwprintf(stdout, L"║");
    }else{
        // Maybe we can center the functions?
        int bytes = 0;
        bytes += fwprintf(stdout, L"║ [%zu] ", idx);
        bytes += fwprintf(stdout, L"Function: %-25s ", fname);
        if(f_status == FNS_UNKNOWN_BEAHAVIOR)
            fwprintf(stdout, L"[U]");
        else if(f_status == FNS_FUNCTION_SUCCED)
            fwprintf(stdout, L"[S]");
        else if(f_status == FNS_FUNCTION_FAILED)
            fwprintf(stdout, L"[F]");
        bytes += 3;
        for(int i = bytes; i < tester_header_length; ++i){
            fwprintf(stdout, L" ");
        }
        fwprintf(stdout, L"║");
    }
    fwprintf(stdout, L"\n");
    if(f_status != FNS_FUNCTION_SUCCED){
        fwprintf(stdout, L"║  ╠══ Result:   %s\n", res);
        fwprintf(stdout, L"║  ╚══ Expected: %s\n", expected);
    }
}


#define START_TEST(...) __impl_test_start(__FILE__)

#define END_TEST(...)   __impl_test_end( )

#define __PRINT_STRING_INFO(self, _func)\
        do{\
            const char* ptr = #self;\
            (void)fprintf(stdout, "%s\n", _func ":");\
                (void)fprintf(stdout, "String info:[%s]:\n", &ptr[1]);\
                (void)fprintf(stdout, "    (ptr): %s\n", (self)->ptr[0] == '\0'? "<EMPTY>" : (self)->ptr);\
                (void)fprintf(stdout, "    (size): %zu\n", String_cap((self)));\
                (void)fprintf(stdout, "    (len): %zu\n\n\n", String_len((self)));\
        }while(0)

#define __PRINT_STR_INFO(ptr)\
        {\
            (void)fprintf( stdout,\
                "String info:[%s]:\n\t(ptr): %s\n\t(len): %zu\n\n\n",\
                #ptr, ptr, strlen(ptr)\
            );\
        }

#define CFUNC(_func)    YELLOW_HIGH_TEXT _func COLOR_RESET
#define SUCCEED         MK_HGREEN_TXT("Succeed")
#define FAILED          MK_BHRED_TXT("Failed")
#define RESULT          MK_HBLUE_TXT("Result")
#define EXPECTED        MK_BHMAGENTA_TXT("Expected")

#define FUNC_SUCCEED(_func)\
        fprintf(stdout, "Function: %-33s => %s!\n   %s -> %s\n",\
                CFUNC(_func), SUCCEED, RESULT, res );

#define FUNC_FAILED(_func)\
        fprintf(stdout, "Function: %-33s => %s!\n   %s -> %s\n",\
            CFUNC(_func), FAILED, EXPECTED, res );

#define __INSERT_TESTER(_func, _str, ...)\
        {\
            (void) _func (&_str, __VA_ARGS__ );\
            CStr res = (&_str)->ptr;\
            if(String_equ_cstr(&_str, "Hello World") == STRING_EQUAL){\
                FUNC_SUCCEED(#_func)\
            }else{\
                FUNC_FAILED(#_func)\
            }\
        }

