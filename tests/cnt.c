#include "__string_tester.h"

#define TOTAL_TESTING_FUNCTION 4

typedef struct {
    String  s;
    int     fn_exit_status;
}Tst_StringRetVal;

typedef struct Tester_string_Parameters {
    CStr             fn_exp;
    CStr             fn_ini;
}TSP;

#define TESTER_FUNCTION(_fn, ...)\
        do{\
            TSP prm = {__VA_ARGS__};\
            Tst_StringRetVal __r = Testing_ ## _fn(prm);\
            __impl_tester_log(#_fn, idx++, __r.s.ptr, prm.fn_exp);\
        }while(0)

static inline Tst_StringRetVal ret_tst(String s, int status){
    return (Tst_StringRetVal){ .s = s, .fn_exit_status = status };
}

Tst_StringRetVal Testing_String_new(TSP prm);
Tst_StringRetVal Testing_String_new_from_cstr(TSP prm);
Tst_StringRetVal Testing_String_new_from_Str(TSP prm);
Tst_StringRetVal Testing_String_new_from_sv(TSP prm);

bool is_atty =              false;
int fn_status[4] =          { false };
const int n_total =         TOTAL_TESTING_FUNCTION;
int tester_header_length =  60;
size_t idx =                0;

int main(void){

    const char * ims = "Hello From String.h";

    START_TEST( );
        TESTER_FUNCTION(String_new,             .fn_exp = "");
        TESTER_FUNCTION(String_new_from_cstr,   .fn_exp = ims,     .fn_ini = ims);
        TESTER_FUNCTION(String_new_from_Str,    .fn_exp = ims,     .fn_ini = ims);
        TESTER_FUNCTION(String_new_from_sv,     .fn_exp = "Hello", .fn_ini = ims);
    END_TEST( );

    return 0;
}

Tst_StringRetVal Testing_String_new(TSP prm) {

    String s1 = String_new( ); /* should return an empty string */
    if(s1.ptr == NULL)             return ret_tst(s1, FNS_FUNCTION_FAILED);
    if(__impl_equstr(s1.ptr, ""))  return ret_tst(s1, FNS_FUNCTION_SUCCED);

    return ret_tst(s1, FNS_UNKNOWN_BEAHAVIOR);
}

Tst_StringRetVal Testing_String_new_from_cstr(TSP prm){
    String s1 = String_new_from_cstr("Hello World from String.h");
    if(s1.ptr == NULL)              return ret_tst(s1, FNS_FUNCTION_FAILED);
    if(__impl_equstr(s1.ptr, prm.fn_exp))  return ret_tst(s1, FNS_FUNCTION_SUCCED);

    return ret_tst(s1, FNS_UNKNOWN_BEAHAVIOR);
}

Tst_StringRetVal Testing_String_new_from_Str(TSP prm){
    String s2 = String_new_from_cstr(prm.fn_ini);
    String s1 = String_new_from_Str(&s2);

    if(s1.ptr == NULL)                      return ret_tst(s1, FNS_FUNCTION_FAILED);
    if(__impl_equstr(s1.ptr, prm.fn_exp))   return ret_tst(s1, FNS_FUNCTION_SUCCED);

    return ret_tst(s1, FNS_UNKNOWN_BEAHAVIOR);
}

Tst_StringRetVal Testing_String_new_from_sv(TSP prm){
    String s1 = String_new_from_sv(prm.fn_ini, 5);
    
    if(s1.ptr == NULL)                      return ret_tst(s1, FNS_FUNCTION_FAILED);
    if(__impl_equstr(s1.ptr, prm.fn_exp))   return ret_tst(s1, FNS_FUNCTION_SUCCED);

    return ret_tst(s1, FNS_UNKNOWN_BEAHAVIOR);
}
