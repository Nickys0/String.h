/** 
 *
 *   ===---------__-*== String.h ==*__----------===
 *      Basic c Library for String manipulation
 *                      v0.1.0
 *   ===-------------*============*-------------===
 * 
 * 
 *   EXAMPLE:
 *   
 *   ```c
 *   #define [OPTIONS]
 *   #include <stdio.h>
 *   #include "String.h" 
 * 
 *   int main(int argc, char** argv){
 *       String str = String_new_from_cstr("Hello, World!\n");
 *       printf("%s", STRARG(str));
 *
 *       return 0;
 *   }
 *   ```
 * 
 *   OPTIONS:
 *   -    #define STRING_IMPLEMENTATION         It is used to the the library to include the implementation of 
 *                                              its self
 * 
 *   -    #define STRING_USE_STD_HANDLERS       If defined instead of using internal implementation of some string.h 
 *                                              functions it will include string.h instead
 *
 *   -    #define STRING_USE_LIBC_ALLOCATORS    It is used to know what allocator should be used [DEFAULT:0(malloc)]
 * 
**/

#ifndef _C_STRING_H
#define _C_STRING_H

#include <errno.h>   // for errno
#include <assert.h>  // for assert
#include <stdbool.h> // for bool, true and false

/* SETTINGS */

/* Possible valus:
	0: malloc, 
	1: calloc 
*/
#ifndef STRING_USE_LIBC_ALLOCATORS
#   define STRING_USE_LIBC_ALLOCATORS  0
#endif
/************/

#ifdef STRING_USE_LIBC_ALLOCATORS
#   include <stdlib.h> // for libc allocators
#   define STRING_FREE(_ptr)            free(_ptr)
#   define STRING_REALLOC(_old, _size)  realloc(_old, _size)
#   if STRING_USE_LIBC_ALLOCATORS == 0
#       define STRING_ALLOC(_nBytes)    malloc( _nBytes )
#   elif STRING_USE_LIBC_ALLOCATORS == 1
#       define STRING_ALLOC(_nBytes)    calloc(_nBytes, sizeof(char))
#   else
#       error "Invalid allocator chosen!"
#   endif /* STRING_USE_LIBC_ALLOCATORS == 0 */

#elif !defined(STRING_ALLOC)
#   error "No allocator chosen!"
#endif  /* defined STRING_USE_LIBC_ALLOCATORS */

#define STR_UNUSED(value) (void)(value)

#ifndef STRING_IMPLEMENTATION
#	if defined(__STDC__)
#		define STRAPI extern
#	elif defined(__cplusplus)
#		define STRAPI extern "C"
#	endif
#else
#	define STRAPI
#endif

#ifndef STRING_CHUNK_SIZE
#   define STRING_CHUNK_SIZE 32
#endif

#define STRING_NO_ERROR     0

#define STR_TODO(message)\
        assert(0 && "TODO: " message)
        
#define STR_ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

#define __ASSERT_IS_VALID_STRING(str)\
        assert(	(str) != NULL &&\
				(str)->ptr != NULL &&\
				"Invalid string!: (NULL)")

#define __ASSERT_SELF           __ASSERT_IS_VALID_STRING(self)
#define __ASSERT_SELF_AND_VALID(_str)\
        __ASSERT_IS_VALID_STRING(self); __ASSERT_IS_VALID_STRING(_str)
#define __ASSERT_SELF_AND(_cond)\
            assert((self) != NULL &&\
				    (self)->ptr != NULL &&\
                    (_cond) &&\
				    "Invalid parameters!")

#ifndef __ch_to_lower
#   define __ch_to_lower(_ch)\
        ((_ch) >= 'A' && (_ch) <= 'Z') ? ((_ch) + 32) : (_ch)
#endif
#ifndef __ch_is_space
#   define __ch_is_space(_ch)\
        ((_ch) == ' ' || (_ch) == '\n' || (_ch) == '\r' || (_ch) == '\t' || (_ch) == '\v') ? true : false
#endif

#ifdef STRING_USE_STD_HANDLERS
#   include <string.h>
#   define STRCPY   strcpy
#   define STRNCAT  strncat
#   define STRCAT   strcat
#   define MEMSET   memset
#   define STREQU(s1, s2)       strcmp(s1, s2) == 0   
#   define STRNEQU(s1, s2, _n)  strncmp(s1, s2, _n) == 0
#   define STRLEN   strlen
#else
#   ifndef STRCPY
#      define __def_strcpy
#      define STRCPY   __impl_strcpy
#   endif
#   ifndef STRNCAT
#      define __def_strncat
#      define STRNCAT  __impl_strncat
#   endif
#   ifndef STRCAT
#      define __def_strcat
#      define STRCAT   __impl_strcat
#   endif
#   ifndef MEMSET
#      define __def_memset
#      define MEMSET   __impl_memset
#   endif
#   ifndef STREQU
#      define __def_strequ
#      define STREQU   __impl_equstr
#   endif
#   ifndef STRNEQU
#      define __def_strnequ
#      define STRNEQU  __impl_nequstr
#   endif
#   ifndef STRLEN
#      define __def_strlen
#      define STRLEN   __impl_strlen
#   endif
#endif


// String as parameter
#define STRARG(_str)    (_str).ptr
#define PSTRARG(_str)   (_str)->ptr

typedef struct __basic_string {
    char*  ptr;
    size_t len;
    size_t cap;
}String;

// typedef struct __basic_string_view {
//     const char *data;
//     size_t      count;
// }String_View;

#define STRING_NOT_EQUAL 0
#define STRING_EQUAL     1

typedef const char*     CStr;       /* \0 terminate string */
typedef const char*     Str;        /* sequence of character with a specifed length */
typedef const String*   RFString;   /* const String pointer used as a reference */
typedef String*         PString;    /* Pointer to String */
typedef int				strerr_t;

 ///////////////////////////////////////
// TODO: apart from the suffix all the names whould be lowercase:
//       String_new_from_Str => String_new_from_rfstr where rf stands for Reference(const*)

//	STRING FUNCTION DECLARATION START //
STRAPI String String_new(void);
STRAPI String String_new_from_cstr(CStr cstr);
STRAPI String String_new_from_sv(Str str, size_t len);
STRAPI String String_new_from_Str(RFString string);

STRAPI size_t String_len(RFString self);
STRAPI size_t String_cap(RFString self);
STRAPI CStr String_data(RFString self);

// STRAPI String String_new_from_file(CStr path); // TODO: This function should only be defined if the user included stdio.h

// This function will check if len + reqBytes is < cap if not it 
// will realloc the string
STRAPI void String_req(PString self, size_t reqBytes);

STRAPI CStr String_app_cstr(PString self, CStr cstr);
STRAPI CStr String_app_sv(PString self, Str str, size_t Nbytes);
/// @brief This function will append a word from the parameter with max_bytes indicating the end of the str (NOT THE WORD)
/// @return An errno error if something went wrong
STRAPI strerr_t String_app_word(PString self, Str word, size_t max_bytes);
/// @param Nbytes Number of bytes to append
/// @param offset Offset of self from self->ptr
/// @return A const reference to the self->ptr
STRAPI CStr String_app_sv_range(PString self, Str _str, size_t Nbytes, size_t offset);
STRAPI CStr String_app_Str(PString self, RFString str); // return a const referce to self->ptr
STRAPI CStr String_app_nStr(PString self, RFString str, size_t Nbytes); /// @return a const referce to self->ptr
STRAPI CStr String_app_Str_range(PString self, RFString str, size_t Nbytes, size_t offset);
STRAPI CStr String_push_char(PString self, char ch);

/// @param str String with length Nbytes
/// @param Nbytes The str length
/// @param start_idx &self->ptr[start_idx] indicates the point where to insert the str
STRAPI void String_ins_cstr(PString self, CStr str, size_t start_idx);
STRAPI void String_ins_sv(PString self, Str str, size_t Nbytes, size_t start_idx);
STRAPI void String_ins_Str(PString self, RFString str, size_t start_idx);
STRAPI void String_ins_Str_range(PString self, RFString str, size_t NBytes, size_t start_idx, size_t stroff);

    /* String removers */
STRAPI strerr_t String_del_word(PString self, CStr word);                   // Fails :: -1(Not found)
STRAPI strerr_t String_del_range(PString self, size_t off, size_t Nbytes);  // Fails :: ERANGE
STRAPI void String_del_word_rcs(PString self, CStr word); // deleter word recurservely
STRAPI CStr String_trim_right(PString self, size_t Nbytes);	// FAILS :: NULL If Nbytes is > than self->len
/// @brief It cuts the string in two: The left part is still in self, 
///         the rigth one is returned
/// @warning The return string is valid untill the self isn't modifed, 
///         it is recomanded to copy it into a buffer
STRAPI CStr String_chop_by_delim(PString self, char d);
/// @return A const pointer to the new originated string
STRAPI CStr String_trim_left(PString self, size_t Nbytes); //FAILS :: NULL If Nbytes > self->len
STRAPI char String_pop_char(PString self);

// Returns a duplicate of the string, the str->ptr is in the heap but the 
// structure its self its on the stack
STRAPI String String_dupl(RFString self);
STRAPI String* PString_dupl(RFString self);
STRAPI char* String_cstr_dupl(RFString self);

STRAPI CStr String_copy(PString self, CStr str);
STRAPI CStr String_ncopy(PString self, Str str, size_t Nbytes);
STRAPI CStr String_copy_str(PString self, RFString str);

STRAPI CStr String_cat(PString self, CStr str);
STRAPI CStr String_ncat(PString self, Str str, size_t Nbytes);
STRAPI CStr String_cat_str(PString self, RFString str);

STRAPI char* String_srch_word(PString self, CStr _word);
STRAPI char* String_srch_word_as_Str(PString self, RFString _word);
STRAPI char* String_srch_nch(PString self, Str str, size_t Nbytes);
STRAPI char* String_srch_ch(PString self, char ch);

// STRAPI strerr_t String_load_file(PString self, CStr path); // TODO: see String_new_from_file
// TODO: *_Str => *_str
STRAPI bool String_equ_cstr(RFString self, CStr str1);
STRAPI bool String_equ_sv(RFString self, Str str1, size_t s1Len);  // comparing self and str1 for Nbytes
STRAPI bool String_equ_Str(RFString self, RFString str1);
STRAPI bool String_equ_oStr(RFString self, RFString str1, size_t offset); // Compares the two string but str1 starts from offset

STRAPI bool String_bequ_oStr(RFString self, RFString str1, size_t str1off, size_t Nbytes); // Compares self with a range of str1
STRAPI bool String_bequ_Str(RFString self, RFString str1, size_t Nbytes); // Compares NBytes of the two strings
#define String_bequ_cstr    String_equ_sv //alias
#define String_bequ_sv     String_equ_sv //alias

// Case insensitive of the prec compare functions
STRAPI bool String_iequ_cstr(RFString self, CStr str1);
STRAPI bool String_iequ_sv(RFString self, Str str1, size_t Nbytes);
STRAPI bool String_iequ_Str(RFString self, RFString str1);
STRAPI bool String_iequ_oStr(RFString self, RFString str1, size_t offset);

STRAPI bool String_ibequ_oStr(RFString self, RFString str1, size_t offset, size_t Nbytes);
STRAPI bool String_ibequ_Str(RFString self, RFString str1, size_t _Nbytes);
#define String_ibequ_cstr    String_iequ_sv //alias
#define String_ibequ_str     String_iequ_sv //alias

STRAPI void String_clear(PString self);
STRAPI void String_unwind(PString self); // sets the len of the string to zero without setting to '\0' all the string
STRAPI void String_shrink_to(PString self, size_t to_len);
STRAPI void String_shrink_for(PString self, size_t Nbytes);
STRAPI void String_free(PString self);
// STRING FUNCTION DECLARATION END //
////////////////////////////////////

////////////////////////////////////////////
// STRING VIEW FUNCTION DECLARATION START //
// Maybe these should be in another header
// STRAPI String_View SV_from_parts(const char *data, size_t count);
// STRAPI String_View SV_from_String(RFString str);
// STRAPI String_View SV_from_cstr(const char *cstr);

// STRAPI String_View SV_trim_left(String_View sv);
// STRAPI String_View SV_trim_right(String_View sv);
// STRAPI String_View SV_trim(String_View sv);
// STRAPI String_View SV_chop_by_delim(String_View *sv, char delim);
// STRAPI String_View SV_chop_by_sv(String_View *sv, String_View thicc_delim);
// STRAPI String_View SV_chop_left(String_View *sv, size_t n);
// STRAPI String_View SV_chop_right(String_View *sv, size_t n);

// STRAPI strerr_t SV_index_of(String_View sv, char c, size_t *index);
// STRAPI strerr_t SV_eq(String_View a, String_View b);
// STRAPI strerr_t SV_eqi(String_View a, String_View b);
// STRAPI strerr_t SV_starts_with(String_View sv, String_View prefix);
// STRAPI strerr_t SV_ends_with(String_View sv, String_View suffix);
// STRING VIEW FUNCTION DECLARATION END //
/////////////////////////////////////////

#ifdef STRING_IMPLEMENTATION

// It's to avoid including string.h
// static size_t __impl_strlen(const char* str1);
// static bool __impl_equstr(const char* str1, const char* str2);

// static char* __impl_strcpy(char* s1, const char* s2);
// static char* __impl_strcat(char* s1, const char* s2);
// static char* __impl_strncat(char* s1, const char* s2, size_t __n);
// static void* __impl_memset(void* s1, int ch, size_t __n);

#ifdef __def_strlen
static size_t __impl_strlen(const char* str1){
    size_t __r = 0;
    for( ;*str1++ != '\0'; ++__r) { }
    return __r;
}
#endif

#ifdef __def_strequ
bool __impl_equstr(const char* str1, const char* str2){
    while(*str1 != '\0' && *str1++ == *str2++){ }
    return (*str1 == '\0' && *str2 == '\0');
}
#endif

#ifdef __def_strnequ
bool __impl_nequstr(const char* s1, const char* s2, size_t Nbytes){
    while(Nbytes-- > 0){
        if(*s1++ != *s2++) return false;
    }
    return true;
}
#endif

#ifdef __def_strcpy
char* __impl_strcpy(char* s1, const char* s2){
    char* s = s1;
    while(*s2 != '\0') *s++ = *s2++;
    *s = '\0';
    return s1;
}
#endif

#ifdef __def_strcat
char* __impl_strcat(char* s1, const char* s2){
    char* s = s1;
    while(*s++ != '\0') { }
    while(*s2 != '\0'){ *s++ = *s2++; }
    *s = '\0';
    return s1;
}
#endif

#ifdef __def_strncat
char* __impl_strncat(char* s1, const char* s2, size_t __n){
    char* s = s1;
    while(*s != '\0') { s++; }
    while(__n-- > 0){ *s++ = *s2++; }
    *s = '\0';
    return s1;
}
#endif

#ifdef __def_memset
void* __impl_memset(void* s1, int ch, size_t __n){
    unsigned char* __r = (unsigned char*) s1;
    for(size_t i = 0; i < __n; ++i) __r[i] = (unsigned char)ch;
    return (void*)__r;
}
#endif

// This function will check if there is enough space for pushing _Nbytes into self, 
// if not it will expand the string
static void _String_fit(PString self, size_t _Nbytes){
    if(self->cap > (self->len + _Nbytes)){
		assert(self->ptr != NULL && "This should not have happened!");
        return;
    }

    size_t tot_bytes = (self->len + _Nbytes);

    // Calculating the new size
    self->cap = (STRING_CHUNK_SIZE * (tot_bytes / STRING_CHUNK_SIZE)) 
                    + (STRING_CHUNK_SIZE * ((tot_bytes % STRING_CHUNK_SIZE) > 0)); 

allocator:
    self->ptr = (char*) STRING_REALLOC(self->ptr, self->cap);
    assert(self->ptr != NULL); /* We don't wann deal with allocation errors */

#if defined(STRING_INIT_MEM_AFTER_ALLOC)
    (void)MEMSET(&self->ptr[self->len], 0, (self->cap - self->len));
#endif /* defined(STRING_INIT_MEM_AFTER_ALLOC) */
}

void String_req(PString self, size_t Nbytes){
    __ASSERT_SELF;
    _String_fit(self, Nbytes);
}


    /* constructors */
String String_new(void){
    String __r = {
        .ptr = STRING_ALLOC(STRING_CHUNK_SIZE),
        .cap = STRING_CHUNK_SIZE,
        .len = 0ULL
    };
    assert(__r.ptr != NULL);
    return __r;
}

String String_new_from_cstr(CStr _cstr){
    return String_new_from_sv(_cstr, STRLEN(_cstr));
}

String String_new_from_sv(Str _str, size_t _Nbytes){
    assert(_str != NULL);
    String ret = String_new( );
    String_ncopy(&ret, _str, _Nbytes);
    return ret;
}

String String_new_from_Str(RFString string){
    assert(string != NULL);

    String ret = {
        .len = string->len,
        .cap = string->cap,
        .ptr = STRING_ALLOC(string->cap) 
    };
    assert(ret.ptr != NULL);

    STRCPY(ret.ptr, string->ptr);
    
    return ret;
}

size_t String_len(RFString self){
    assert(self != NULL && "Invalid Parameter!");
    return self->len;
}

size_t String_cap(RFString self){
    assert(self != NULL && "Invalid Parameter!");
    return self->cap;
}

CStr String_data(RFString self){
    assert(self != NULL && "Invalid Parameter!");
    return self->ptr;
}


// String String_new_from_file(CStr path){
//     assert((path != NULL) && "Invalid param");
//     String ret = {0};
//     if(!path) return ret;
//     ret.cap = STRING_CHUNK_SIZE;
//     String_load_file(&ret, path);
//     return ret;
// }
    /******************/

    /* appenders */
CStr String_app_cstr(PString self, CStr _cstr){
    return String_app_sv( self, _cstr, STRLEN(_cstr) );
}

CStr String_app_sv(PString self, Str _str, size_t Nbytes){
    __ASSERT_SELF_AND(_str != NULL);
    
    _String_fit(self, Nbytes);
    STRNCAT(self->ptr, _str, Nbytes);
        
    self->len += Nbytes;

    return self->ptr;
}

strerr_t String_app_word(PString self, Str word, size_t max_bytes){
    __ASSERT_SELF_AND(word != NULL);

    _String_fit(self, (max_bytes + 1));

    self->ptr[self->len++] = (char)32;
    self->ptr[self->len] = (char)0; /* ensuring a null terminating string */

    char* ptr = &self->ptr[self->len];

    size_t i = 0;
    for(; i < max_bytes; i++){
        if(*word == (char)32) break;

        *ptr++ = *word++;
        assert(i + self->len < self->cap);
    }
        
    self->len += i;
    self->ptr[self->len] = '\0';

    return STRING_NO_ERROR;
}

CStr String_app_sv_range(PString self, Str _str, size_t Nbytes, size_t offset){
    __ASSERT_SELF_AND(_str != NULL);
    
    _String_fit(self, Nbytes);
    STRNCAT(self->ptr, &_str[offset], Nbytes);
    
    self->len += Nbytes;

    return self->ptr;
}

CStr String_app_Str(PString self, RFString _str){
    __ASSERT_SELF_AND_VALID(_str);
    
    _String_fit(self, _str->len); /* self should at least be _str->len bytes */
    STRNCAT(self->ptr, _str->ptr, _str->len);

    return self->ptr;
}

CStr String_app_nStr(PString self, RFString _str, size_t _Nbytes){
    __ASSERT_SELF_AND_VALID(_str);

    _String_fit(self, _Nbytes);
    STRNCAT(self->ptr, _str->ptr, _Nbytes);
    self->len += _Nbytes;
    
    return self->ptr;
}

CStr String_app_Str_range(PString self, RFString _str, size_t _Nbytes, size_t _offset){
    __ASSERT_SELF_AND_VALID(_str);
    
    _String_fit(self, _Nbytes);
    STRNCAT(self->ptr, &_str->ptr[_offset], _Nbytes);
    self->len += _Nbytes;
    
    return self->ptr;
}

CStr String_push_char(PString self, char ch){
    __ASSERT_SELF;

    _String_fit(self, 1ULL);
    self->ptr[self->len++] = ch;
    self->ptr[self->len] = '\0';

    return self->ptr;
}
/***********************/

    /* Inserters */
void String_ins_cstr(PString self, CStr str, size_t start_idx){
    String_ins_sv(self, str, STRLEN(str), start_idx);
}


void String_ins_Str(PString self, RFString str, size_t start_idx){
    String_ins_sv(self, str->ptr, str->len, start_idx);
}

void String_ins_Str_range(PString self, RFString str, size_t NBytes, size_t start_idx, size_t off){
    String_ins_sv(self, &str->ptr[off], NBytes, start_idx);
}

void String_ins_sv(PString self, Str str, size_t Nbytes, size_t start_idx){
    __ASSERT_SELF_AND(str != NULL);
    assert(self->cap > start_idx);

    _String_fit(self, Nbytes);

    //      lch
    //       v
    //HelWorld00000000   -> self->ptr
    //   ^      ^
    // ins   nw_lch
    //lo 0000000000000   -> str[..Nbytes]
    const char* lch = &self->ptr[self->len - 1]; // This points to the last character(the one before '\0')
    char* nw_lch =   &self->ptr[(self->len + Nbytes) - 1]; // This points to the new last character
    char* ins = &self->ptr[start_idx]; // where to insert str

    //Hel|Wor|ld00000000   -> self->ptr
    //Hel|Wor|World00000   -> self->ptr
    //Hel|lo |World00000   -> self->ptr

    // we move the string
    do{
        *nw_lch-- = *lch;
    }while(lch-- != ins);
    
    // we copy the string
    while(Nbytes-- > 0) {
        *ins++ = *str++;
    }
    
    self->len += Nbytes;
}
    /*************/


    /* Utility */
String String_dupl(RFString str){
    assert(str != NULL && "Invalid parameter passed!"); 
    return String_new_from_Str(str);
}

String* PString_dupl(RFString str){
    __ASSERT_IS_VALID_STRING(str);

    String* ret = (String*)STRING_ALLOC(sizeof(String));

    ret->len = str->len;
    ret->cap = str->cap;
    ret->ptr = STRING_ALLOC( ret->cap );
    assert(ret->ptr != NULL);
    
    STRCPY(ret->ptr, str->ptr);

    return ret;
}

char* String_cstr_dupl(RFString self){
    __ASSERT_SELF;
    char* __r = STRING_ALLOC(self->len);
    return STRCPY(__r, self->ptr);
}

// TODO: return NULL or char*;
CStr String_copy(PString self, CStr str){
    __ASSERT_SELF_AND(str != NULL);

    String_unwind(self);
    size_t lstr = STRLEN(str);
    _String_fit(self, lstr);
    self->len = lstr;
    
    char* s = self->ptr;
    while(*str != '\0'){ *s++ = *str++; }
    *s = '\0';

    return self->ptr;
}

CStr String_ncopy(PString self, Str str, size_t Nbytes){
    __ASSERT_SELF_AND(str != NULL);
    
    String_unwind(self);
    _String_fit(self, Nbytes);
    self->len = Nbytes;
    
    char* ptr = self->ptr;
    while(Nbytes-- > 0){ *ptr++ = *str++; }
    *ptr = '\0';

    return self->ptr;
}

CStr String_copy_str(PString self, RFString str){
    __ASSERT_SELF_AND_VALID(str);
    return String_copy(self, str->ptr);
}

CStr String_cat(PString self, CStr str){
    __ASSERT_SELF_AND(str != NULL);
    
    size_t lstr = STRLEN(str);
    _String_fit(self, lstr);

    char* s = &self->ptr[self->len];
    self->len += lstr;

    return STRCPY(s, str);
}

CStr String_ncat(PString self, Str str, size_t Nbytes){
    __ASSERT_SELF_AND(str != NULL);

    _String_fit(self, Nbytes);

    char* s = &self->ptr[self->len];
    self->len += Nbytes;

    while(Nbytes-- > 0){ *s++ = *str++; }

    return self->ptr;
}

char* String_srch_word(PString self, CStr _word){
    __ASSERT_SELF_AND(_word != NULL);

    char* ret = self->ptr;
    size_t wlen = STRLEN(_word) - 1; /* we don't need to count the '\0' */

    do{
        if(*ret == *_word && STRNEQU(ret, _word, wlen)){
            /* if the last character is a white space 
                it means it is a valid word */
            if(__ch_is_space(ret[wlen + 1])) 
                return ret;
        }
    }while(*++ret != '\0');

    return NULL;
}

char* String_srch_ch(PString self, char ch){
    __ASSERT_SELF;
    
    char* ret = self->ptr; 

    do{
        if(*ret == ch) return ret;
    }while(*++ret != '\0');

    return NULL;
}

char* String_srch_nch(PString self, Str str, size_t Nbytes){
    __ASSERT_SELF_AND(str != NULL);

    char* ret = self->ptr;
    size_t i =  0;

    do{
        while(*ret++ == *str){
            if(++i >= Nbytes) return ret; 
            str++; 
        }
    }while(*ret != '\0');

    return ret;
}

char* String_srch_word_as_Str(PString self, RFString _word){
    __ASSERT_SELF_AND_VALID(_word);

    char* ret = self->ptr; 

    do{
        // space    v_______v
        if(*ret == (char)32) continue; /* do wee need this */

        if(*ret == *(_word->ptr))
            if(STREQU(ret, _word->ptr)) 
                return ret;
    }while(*++ret != '\0');

    return NULL;
}

// TODO: this function should only show up if stdio.h was include from the user
// strerr_t String_load_file(PString self, CStr path){
//     __ASSERT_SELF_AND(path != NULL);
//     FILE* file = fopen(path, "w");
//     if(file == NULL)
//         return errno;
//     fseek(file, 0L, SEEK_END);
//     long file_size = ftell(file);
//     if(file_size == -1)
//         return errno;
//     fseek(file, 0L, SEEK_CUR);
//     _String_fit(self, (size_t)file_size);
//     fread(self->ptr, sizeof(char), file_size, file);
//     fclose(file);
//     return STRING_NO_ERROR;
// }

strerr_t String_del_word(PString self, CStr word){
    __ASSERT_SELF_AND(word != NULL);

    char* ref_word = String_srch_word(self, word);
    if(ref_word == NULL) return -1; /* word not found */

    size_t wlen = STRLEN(word) + 1;
    char* nword = &ref_word[wlen];
    self->len -= wlen;

    // we overwrite the word with the rest of the string
    while(wlen-- > 0){
        *ref_word++ = *nword++;
    }
    *ref_word = '\0'; /* ensuring null-terminated string */

    assert(STRLEN(self->ptr) == self->len && "Something was calculated wrong");
    return STRING_NO_ERROR;
}

void String_del_word_rcs(PString self, CStr word){
    __ASSERT_SELF_AND(word != NULL);

    char* ref_word = self->ptr;
    const size_t wlen = STRLEN(word);

    do{
        // just the word without the trailing '\0'
        if(*ref_word == *word && STRNEQU(ref_word, word, wlen - 1)){
            if(((ref_word - self->ptr) + (wlen + 1)) > self->len)
                String_del_range(self, (ref_word - self->ptr) - 1, wlen + 1);
            else
                String_del_range(self, (ref_word - self->ptr), wlen + 1);
        }
    }while(*ref_word++ != '\0');
    
    assert(STRLEN(self->ptr) == self->len && "Something was miscalculated!");
}

CStr String_trim_right(PString self, size_t Nbytes){
    __ASSERT_SELF;

    if(self->len < Nbytes) return NULL;

    if(self->len > 0){
        self->len -= Nbytes;
        self->ptr[self->len] = '\0';
    }

    return self->ptr;
}

CStr String_chop_by_delim(PString self, char d){
    __ASSERT_SELF;
    
    char* ptr = self->ptr;

    while(*ptr != '\0') {
        if(*ptr == d){
            *ptr = '\0';
            self->len = ptr - self->ptr;
            break;
        }
        ptr++;
    }

    return &self->ptr[self->len + 1];
}

CStr String_trim_left(PString self, size_t Nbytes){
    __ASSERT_SELF;
    assert(self->cap > Nbytes && "Out Of Bounds!");
    
    if(Nbytes > self->len) return NULL;

    char* ptr =         self->ptr;
    const char* nwptr = &self->ptr[Nbytes];
    self->len -= Nbytes;
    
    while (Nbytes-- > 0){
        *ptr++ = *nwptr++;
    }
    *ptr = '\0';
    
    assert(STRLEN(self->ptr) == self->len && "Something was miscalculated!");
    return self->ptr;
}

char String_pop_char(PString self){
    __ASSERT_SELF;
    if(self->len == 0) return 0;

    char ret = self->ptr[--self->len];
    self->ptr[self->len] = '\0';
    return ret;
}

strerr_t String_del_range(PString self, size_t off, size_t Nbytes){
    __ASSERT_SELF;
    assert(self->cap > (off + Nbytes) && "Out of bounds");

    if(off >= self->len || Nbytes == 0) return STRING_NO_ERROR;
    
    char* beg = &self->ptr[off];
    char* end = &beg[Nbytes];
    while(*end != '\0'){
        *beg++ = *end++;
    }
    self->len -= (end - beg);
    *beg = '\0'; /* ensuring null-terminated string */
    assert(STRLEN(self->ptr) == self->len && "Something was miscalculated!");
    
    return STRING_NO_ERROR;
}

    /* comparator */
bool String_equ_Str(RFString self, RFString str1){
    __ASSERT_SELF_AND_VALID(str1);
    return String_equ_cstr(self, str1->ptr);
}

bool String_iequ_Str(RFString self, RFString str1){
    __ASSERT_SELF_AND_VALID(str1);
    return String_iequ_cstr(self, str1->ptr);
}

bool String_iequ_oStr(RFString self, RFString str1, size_t offset){
    __ASSERT_SELF_AND_VALID(str1);
    assert(str1->cap > offset && "OUT OF BOUNDS");

    const char* ptr1 = &self->ptr[offset];
    const char* ptr2 = str1->ptr;
    
    while(*ptr1 != '\0' && ((__ch_to_lower(*ptr1)) == (__ch_to_lower(*ptr2)))){
        ptr1++; ptr2++;
    }
    
    return ((*ptr1 == '\0') && (*ptr2 == '\0'));
}

bool String_ibequ_Str(RFString self, RFString str1, size_t _Nbytes){
    __ASSERT_SELF_AND_VALID(str1);
    return String_iequ_sv(self, str1->ptr, _Nbytes);
}

bool String_ibequ_oStr(RFString self, RFString str1, size_t offset, size_t Nbytes){
    __ASSERT_SELF_AND_VALID(str1);
    assert(offset + Nbytes < str1->cap && "Out of bounds");

    return String_ibequ_str(self, &str1->ptr[offset], Nbytes);
}

bool String_bequ_Str(RFString self, RFString str1, size_t Nbytes){
    __ASSERT_SELF_AND_VALID(str1);
    
	const char* ptr1 = self->ptr;
	const char* ptr2 = str1->ptr;

    while(Nbytes-- > 0){ 
        if(*ptr1 != *ptr2) return STRING_NOT_EQUAL;
    }
	
    return STRING_EQUAL;
}

bool String_equ_oStr(RFString self, RFString str1, size_t offset){
	__ASSERT_SELF_AND_VALID(str1);

	if(offset < str1->len){
		const char* ptr1 = self->ptr;
		const char* ptr2 = &str1->ptr[offset];
	
		do{
			if(*ptr1 != *ptr2) return STRING_NOT_EQUAL;
		}while(*++ptr1 != '\0' && *++ptr2 != '\0');
	}
    
    return STRING_EQUAL;
}

bool String_equ_cstr(RFString self, CStr str1){
	__ASSERT_SELF_AND(str1 != NULL);
    
    const char* ptr = self->ptr;
    while(*ptr != '\0' && *ptr++ == *str1++){ }

    return ((*ptr == '\0') && (*str1 == '\0'));
}

bool String_iequ_cstr(RFString self, CStr str1){
	__ASSERT_SELF_AND(str1 != NULL);
	const char* ptr = self->ptr;
	
    while(*ptr != '\0' && ((__ch_to_lower(*ptr)) == (__ch_to_lower(*str1)))){
        ptr++; str1++;
    }
    
    return ((*ptr == '\0') && (*str1 == '\0'));
}

bool String_equ_sv(RFString self, Str str1, size_t Nbytes){
    __ASSERT_SELF_AND(str1 != NULL);

	const char* ptr = self->ptr;
	while(Nbytes-- > 0){
        if(*str1++ != *ptr++) return STRING_NOT_EQUAL;
    }
    return STRING_EQUAL;
}

bool String_iequ_sv(RFString self, Str str1, size_t Nbytes){
	__ASSERT_SELF_AND(str1 != NULL);

    const char* ptr = self->ptr;
    const char* str = str1;

    while(Nbytes-- > 0){
        if((__ch_to_lower(*ptr)) != (__ch_to_lower(*str))) return STRING_NOT_EQUAL;
    }

    return STRING_EQUAL;
}

bool String_bequ_oStr(RFString self, RFString str1, size_t offset, size_t Nbytes){
    __ASSERT_SELF_AND_VALID(str1);
    assert((offset + Nbytes) < str1->cap && "Out of bounds");

    return String_bequ_sv(self, &str1->ptr[offset], Nbytes);
}
    /***********/

    /* cleaners */
void String_clear(PString self){
    __ASSERT_SELF;

	MEMSET(self->ptr, 0, self->len);
	self->len = 0;
}

void String_free(PString self){
    __ASSERT_SELF;

    if(self->ptr){
        STRING_FREE(self->ptr);
        self->len = 0;
        self->cap = 0;
    }
}

void String_unwind(PString self){
    __ASSERT_SELF;
    self->len = 0;
}

void String_shrink_to(PString self, size_t to_len){
    __ASSERT_SELF_AND(self->cap > to_len && "Out of bounds!");
    self->len = to_len;
    self->ptr[self->len] = '\0';
}

void String_shrink_for(PString self, size_t Nbytes){
    __ASSERT_SELF_AND(self->cap > Nbytes && "Out of bounds!");

    self->len -= Nbytes;
    self->ptr[self->len] = '\0';
}

    ////////////////////////////////////
	//	SV FUNCTION DECLARATION START //

	//	SV FUNCTION DECLARATION END //
	/////////////////////////////////
	/************/

#endif

#undef __ASSERT_IS_VALID_STRING
#undef __ASSERT_SELF
#undef __ASSERT_SELF_AND
#undef STR_UNUSED
#undef STR_TODO
#undef STR_ARRAY_LEN    // Maybe we should not undef this
#undef STRCPY
#undef STRNCAT

#endif

/**
 * 
 *                      -----------------------------
 *                          -*- ANTER LIBRARY -*-
 *                      -----------------------------    
 *  
 *      Version Conventions:
 *      - Modifying comments does not update the version.
 *      - PATCH is incremented in case of a bug fix or refactoring without touching the API.
 *      - MINOR is incremented when new functions and/or types are added in a way that does
 *        not break any existing user code.
 *      - MAJOR update should be just a periodic cleanup of the deprecated functions and types
 *        without really modifying any existing functionality.
 * 
 *      
 *      VERSION HISTORY
 * 
 *      |    DATE    |  VERSION  |              DESCRIPTION
 *      |            |           |
 *      | (26/08/25) |   0.1.1   | ++ Some fixes about the macros that could cause some warnings
 *      |            |           | ++ New method for caluclating the necessary space
 *      |            |           |
 *      | (24/08/25) |   0.1.0   | First release of the library
 *      |            |           |
 *      |            |           |
 * 
 **/

// TODO: add support for wstrings
