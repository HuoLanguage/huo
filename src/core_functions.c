#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"
#include "core_functions.h"
#include "config.h"

void print(struct Value a){
    if(a.type == TYPE_STRING){
        struct String str = value_as_string(&a);
        printf("\"%s\"", string_to_chars(&str));
    }
    else if(a.type == TYPE_KEYWORD){
        struct String kwd = value_as_keyword(&a);
        printf("%s", string_to_chars(&kwd));
    }
    else if(a.type == TYPE_LONG) {
        printf("%" PRIhi, value_as_long(&a));
    }
    else if(a.type == TYPE_FLOAT) {
        printf("%f", value_as_float(&a));
    }
    else if(a.type == TYPE_BOOL) {
        if (value_as_bool(&a)){
            printf("True");
        }
        else{
            printf("False");
        }
    }
    else if(a.type == TYPE_ARRAY) {
        printf("[ ");
        struct Value_array *arr = value_as_array(&a);
        for(size_t i = 0; i < arr->size; i++){
            print(*arr->values[i]);
            if(i < arr->size-1){
              printf(", ");
            }
        }
        printf(" ]");
    }
    else if(a.type == TYPE_UNDEF) {
        printf("undefined");
    }
}

struct Value add(struct Value a, struct Value b){
    if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_long(value_as_long(&a) + value_as_long(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_float(value_as_float(&a) + value_as_float(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_LONG){
        return value_from_float(value_as_float(&a) + (float)value_as_long(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_FLOAT){
        return value_from_float((float)value_as_long(&a) + value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for add: %d != %d", a.type, b.type);
    }
}

struct Value mul(struct Value a, struct Value b){
    if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_long(value_as_long(&a) * value_as_long(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_float(value_as_float(&a) * value_as_float(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_LONG){
        return value_from_float(value_as_float(&a) * (float)value_as_long(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_FLOAT){
        return value_from_float((float)value_as_long(&a) * value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for add: %d != %d", a.type, b.type);
    }
}

struct Value sub(struct Value a, struct Value b){
    if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_long(value_as_long(&a) - value_as_long(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_float(value_as_float(&a) - value_as_float(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_LONG){
        return value_from_float(value_as_float(&a) - (float)value_as_long(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_FLOAT){
        return value_from_float((float)value_as_long(&a) - value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for sub: %d != %d", a.type, b.type);
    }
}

struct Value divide(struct Value a, struct Value b) {
    if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        long bl = value_as_long(&b);
        if (bl == 0) {
            ERROR("Division by 0");
        }
        return long_divide(value_as_long(&a), bl);
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        long bf = value_as_float(&b);
        if (bf == 0) {
            ERROR("Division by 0");
        }
        return value_from_float(value_as_float(&a) / bf);
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_LONG){
        long bl = value_as_long(&b);
        if (bl == 0) {
            ERROR("Division by 0");
        }
        return value_from_float(value_as_float(&a) / (float) bl);
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_FLOAT){
        long bf = value_as_float(&b);
        if (bf == 0) {
            ERROR("Division by 0");
        }
        return value_from_float((float) value_as_long(&a) / bf);
    }
    else {
        ERROR("Mismatched types for divide: %d != %d", a.type, b.type);
    }
}


struct Value long_divide(huo_int_t a, huo_int_t b) {
    if (a % b == 0) {
        return value_from_long(a / b);
    } else {
        return value_from_float((float) a /  (float) b);
    }
}

struct Value concat(struct Value a, struct Value b){
    if (a.type == TYPE_STRING && b.type == TYPE_STRING) {
        return string_concat(value_as_string(&a), value_as_string(&b));
    } else if (a.type == TYPE_ARRAY && b.type == TYPE_ARRAY) {
        return array_concat(value_as_array(&a), value_as_array(&b));
    } else {
        ERROR("Tried to concat %c and %c", a.type, b.type);
    }
}

struct Value array_concat(struct Value_array *a, struct Value_array *b) {
    return value_from_array(array_concat_heap(a, b));
}
struct Value string_concat(struct String a, struct String b) {
    return value_from_string(string_concat_stack(&a, &b));
}

struct Value not(struct Value a, struct Value b){
    if(a.type == TYPE_BOOL && b.type == TYPE_BOOL){
        return value_from_bool(value_as_bool(&a) != value_as_bool(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_bool(value_as_float(&a) != value_as_float(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_bool(value_as_long(&a) != value_as_long(&b));
    }
    else if(a.type == TYPE_STRING && b.type == TYPE_STRING){
        return value_from_bool(!string_matches_stack(value_as_string(&a), value_as_string(&b)));
    }
    else if(a.type == TYPE_ARRAY && b.type == TYPE_ARRAY){
        return value_from_bool(!array_matches(value_as_array(&a), value_as_array(&b)));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}

struct Value and(struct Value a, struct Value b){
    if(a.type == TYPE_BOOL && b.type == TYPE_BOOL){
        return value_from_bool(value_as_bool(&a) && value_as_bool(&b));
    } else {
        ERROR("& operator only takes boolean values: %d | %d != 1", a.type, b.type);
    }
}

struct Value or(struct Value a, struct Value b){
    if(a.type == TYPE_BOOL && b.type == TYPE_BOOL){
        return value_from_bool(value_as_bool(&a) || value_as_bool(&b));
    } else {
        ERROR("| operator only takes boolean values: %d | %d != 1", a.type, b.type);
    }
}

struct Value equals(struct Value a, struct Value b){
    if(a.type == TYPE_BOOL && b.type == TYPE_BOOL){
        return value_from_bool(value_as_bool(&a) == value_as_bool(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_bool(value_as_float(&a) == value_as_float(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_bool(value_as_long(&a) == value_as_long(&b));
    }
    else if(a.type == TYPE_STRING && b.type == TYPE_STRING){
        return value_from_bool(string_matches_stack(value_as_string(&a), value_as_string(&b)));
    }
    else if(a.type == TYPE_ARRAY && b.type == TYPE_ARRAY){
        return value_from_bool(array_matches(value_as_array(&a), value_as_array(&b)));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_LONG){
        return value_from_bool(value_as_float(&a) == (float)value_as_long(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_FLOAT){
        return value_from_bool((float)value_as_long(&a) == value_as_float(&b));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}

struct Value greater_than(struct Value a, struct Value b){
    if(a.type == TYPE_BOOL && b.type == TYPE_BOOL){
        return value_from_bool(value_as_bool(&a) > value_as_bool(&b));
    }
    else if(a.type == TYPE_FLOAT && b.type == TYPE_FLOAT){
        return value_from_bool(value_as_float(&a) > value_as_float(&b));
    }
    else if(a.type == TYPE_LONG && b.type == TYPE_LONG){
        return value_from_bool(value_as_long(&a) > value_as_long(&b));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}

bool can_cast_to_size_t(huo_int_t i) {
    return i >= 0 && (huo_uint_t) i <= SIZE_MAX;
}

struct Value set(struct Value index_val, struct Value item, struct Value *to_set) {
    huo_int_t index = value_as_long(&index_val);
    if (!can_cast_to_size_t(index)) {
		unsigned long long size_max = SIZE_MAX;
        ERROR("Index out of range for set: should be 0 <= %" PRIhi " < %llu", index, size_max);
    }
    if (to_set->type == TYPE_ARRAY) {
        return value_from_array(array_set((size_t) index, item, value_as_array(to_set)));
    } else if (to_set->type == TYPE_STRING) {
        struct String set = string_set((size_t) index, value_as_string(&item), value_as_string(to_set));
        return value_from_string(string_copy_stack(&set));
    } else {
        ERROR("Set type invalid:  ('%c' != [TYPE_ARRAY, TYPE_STRING])", to_set->type);
    }
}

struct Value_array *array_set(size_t index, struct Value item, struct Value_array *array) {


    // Have to copy before incrementing size or else
    // recursive copies segfault
    struct Value *val = value_copy_heap(&item);

    if (index >= array->size) {
        RESIZE(array->values, index + 1);
        struct Value undef = {.type=TYPE_UNDEF};
        for (size_t i = array->size; i < index; i++) {
            array->values[i] = value_copy_heap(&undef);
        }
        array->size = index + 1;
    }
    array->values[index] = val;
    return array;
}

struct String string_set(size_t index, struct String item, struct String string){
    char c = string_index(&item, 0);
    if (index == string_length(&string)) {
        struct String char_str = string_from_char(c);
        string_concat_to(&string, &char_str);
    } else if (index < string_length(&string)) {
        string.body[index] = c;
    } else {
        ERROR("Invalid index: %zu > %zu", index, string_length(&string));
    }
    return string;
}

struct Value push(struct Value what, struct Value where) {
    if (where.type == TYPE_ARRAY) {
        return value_from_array(array_push(what, value_as_array(&where)));
    } else if (where.type == TYPE_STRING) {
        return value_from_string(string_push(value_as_string(&what), value_as_string(&where)));
    } else {
        ERROR("Push type invalid:  ('%c' != [TYPE_ARRAY, TYPE_STRING])", where.type);
    }
}

struct Value_array *array_push(struct Value a, struct Value_array *array){
    RESIZE(array->values, array->size + 1);
    array->values[array->size] = value_copy_heap(&a);
    array->size++;
    return array;
}

struct String string_push(struct String a, struct String str){
    if (string_length(&a) != 1) {
        ERROR("Character does not have length 1");
    }
    return string_concat_stack(&str, &a);
}

struct Value substring(struct Value start, struct Value end, struct Value what) {
    huo_int_t start_i = value_as_long(&start);
    huo_int_t end_i = value_as_long(&end);
    if (start_i < 0 || (huo_uint_t) start_i >= length(what)) {
        ERROR("Start index out of range for substring: should be 0 <= %" PRIhi " < %zu", start_i, length(what));
    }
    if (end_i < 0 || (huo_uint_t) end_i >= length(what)) {
        ERROR("End index out of range for substring: should be 0 <= %" PRIhi " < %zu", end_i, length(what));
    }

    return substring_ll((size_t) start_i, (size_t) end_i, what);
}

struct Value substring_ll(size_t start_i, size_t end_i, struct Value what) {
    //if (what.type == TYPE_ARRAY) {
    //    return value_from_array(array_slice(start_i, end_i, value_as_array(&what)));
    //} else
    if (what.type == TYPE_STRING) {
        return value_from_string(string_substring(start_i, end_i, value_as_string(&what)));
    } else {
        ERROR("Substring type invalid:  ('%c' != [TYPE_ARRAY, TYPE_STRING])", what.type);
    }
}

struct String string_substring(size_t start, size_t end, struct String str){
    assert(string_is_sane(&str));
    if (end > start) {
        struct String result = {
            .length = (end - start),
            .body = ARR_MALLOC(string_length(&str) + 1, char)
        };
        for(size_t i = 0; i < result.length; i++){
            result.body[i] = str.body[i + start];
        }
        result.body[result.length] = 0;
        assert(string_is_sane(&result));
        return result;
    } else {
        return string_from_chars(NULL);
    }
}

struct Value split(struct Value sep, struct Value what) {
    return string_split(value_as_string(&sep), value_as_string(&what));
}

struct Value string_split(struct String sep, struct String what) {

    assert(string_is_sane(&sep));
    assert(string_is_sane(&what));

    size_t c = 0;
    for(size_t i = 0; i < what.length; i++){
        if(sep.length == 0 || sep.body[0] == what.body[i]){
            c += 1;
        }
    }

    if (c == SIZE_MAX) {
        ERROR("Cannot split string into more than size_t pieces");
    }

    size_t *indexes = NULL;
    RESIZE(indexes, c);
    size_t d = 0;
    for(size_t i = 0; i < what.length; i++){
        if(sep.length == 0 || sep.body[0] == what.body[i]){
            indexes[d++] = i;
        }
    }
    assert (c == d);
    struct Value_array * array = malloc_or_die(sizeof(struct Value_array));
    array->values = NULL;
    RESIZE(array->values, c + 1);
    array->size = 0;
    for(size_t l = 0; l <= c; l++){
        size_t start = !l ? l : indexes[l-1] + 1;
        size_t end = (l < c) ? indexes[l] : what.length;

        struct Value item = value_from_string(string_substring(start, end, what));

        array->values[array->size] = value_copy_heap(&item);
        array->size++;
    }
    return value_from_array(array);
}


struct Value value_index(struct Value index, struct Value list) {
    huo_int_t i = value_as_long(&index);
    if(list.type == TYPE_ARRAY){
        struct Value_array *a = value_as_array(&list);
        if (i < 0) {
            ERROR("Negative index: %" PRIhi, i);
        }
        size_t len = a->size;
        if ((huo_uint_t) i >= len) {
            ERROR("Invalid index: %" PRIhi " (len %zu)", i, len);
        }
        return *a->values[i];
    }
    else if(list.type == TYPE_STRING){
        struct String s = value_as_string(&list);
        struct String indexed_char = string_from_char(string_index(&s, i));
        return value_from_string(string_copy_stack(&indexed_char));
    } else {
        ERROR("Index takes a string or array, but got '%c' != [TYPE_ARRAY|TYPE_STRING]).", list.type);
    }
}
