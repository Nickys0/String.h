/**
 *
 *  WARNING:    to run this code your terminal should
 *              accept ansi color
 *
**/
#include "__string_tester.h"

/**
 *
 *     FUNCTIONS TO BE TESTED
 *  => [V] String_del_word(PString self, CStr word)
 *  => [V] String_del_word_rcs(PString self, CStr word)
 *  => [V] String_pop_char(PString self)
 *  => [V] String_del_range(PString self, size_t off, size_t Nbytes)
 *  => [X] String_trim_right(PString self, size_t Nbytes)
 *  => [X] String_chop_by_delim(PString self, char d)
 *  => [X] String_trim_left(PString self, size_t Nbytes)
 *  => [X] String_pop_char(PString self)
 * 
**/

#define TEST_FN(fn, s, ...)\
        do{\
            fn(&(s) __VA_ARGS__);\
            if(STREQU((s).ptr, __exp))\
                 fn_status[idx] = FNS_FUNCTION_SUCCED;\
            else fn_status[idx] = FNS_FUNCTION_FAILED;\
            __impl_tester_log(#fn, idx++, (s).ptr, __exp);\
        }while(0)

#define FUNC_COUNT 8
bool is_atty = false;
const int n_total = FUNC_COUNT;
int fn_status[FUNC_COUNT];
int tester_header_length = 60;

int main(void){
    MEMSET(fn_status, FNS_FUNCTION_FAILED, sizeof(fn_status));
    String s1 = String_new_from_cstr("Hello Please World!!");
	String s2 = String_new_from_cstr("Hello Please World! Please");
	String s3 = String_new_from_cstr("Help me/Hello World"); 
    size_t idx = 0;
    const char* __exp = NULL;
    START_TEST( );
        // TODO: maybe the del_word function should remove the space based on the word position:
        // Ex:
        //  "Help me please" => "Help me" != "Help me "
        __exp = "Hello World!!";
        TEST_FN(String_del_word, s1, ,"Please");
        
        __exp = "Hello World!";
        TEST_FN(String_pop_char, s1);

        __exp = "Hello!";
        TEST_FN(String_del_range, s1, ,5, 6);
        
        __exp = "Hello World!";
        TEST_FN(String_del_word_rcs, s2, ,"Please");

        __exp = "Hello";
        TEST_FN(String_trim_right, s2, ,sizeof("World!"));

        __exp = "Left: Help me | Rigth: Hello World";
        const char* __r = String_chop_by_delim(&s3, '/');
        if(STREQU(s3.ptr, "Help me") && STREQU(__r, "Hello World"))
                fn_status[idx] = FNS_FUNCTION_SUCCED;
        else    fn_status[idx] = FNS_FUNCTION_FAILED;
        __impl_tester_log("String_chop_by_delim", idx++, s3.ptr, __exp);
        
        __exp = "me";
        TEST_FN(String_trim_left, s3, ,sizeof("Help"));
        
        __exp = "m";
        TEST_FN(String_pop_char, s3);

    END_TEST( );
    
    return 0;
}
