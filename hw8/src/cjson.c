#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"
#define MAX_KEY_VALUE_LENGTH 100

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

static PyObject* cjson_loads(PyObject* self, PyObject* args) {

     const char *json_str;

    if (!PyArg_ParseTuple(args, "s", &json_str)) 
        return NULL;  

    PyObject *dict = PyDict_New();
    if (!dict) {
        printf("ERROR: Failed to create Dict Object\n");
        return NULL;
    }

    char key[MAX_KEY_VALUE_LENGTH];
    char value[MAX_KEY_VALUE_LENGTH];
    int key_index = 0, value_index = 0;
    int i = 0;
    int len = strlen(json_str);

    while (isspace(json_str[i])) 
        i++;

    if (json_str[i] != '{') {
        printf("ERROR: JSON должен начинаться с '{'\n");
        return NULL;
    }

    i++; 

    while (i < len) {
        while (isspace(json_str[i])) 
            i++;

        if (json_str[i] == '"') {
            i++;
            key_index = 0;
            while (json_str[i] != '"' && i < len) {
                key[key_index++] = json_str[i++];
            }
            key[key_index] = '\0'; 
            i++;
        } else {
            printf("ERROR: Ожидалась строка в кавычках для ключа.\n");
            return NULL;
        }

        while (isspace(json_str[i])) 
            i++;

        if (json_str[i] != ':') {
            printf("ERROR: Ожидался символ ':' между ключом и значением.\n");
            return NULL;
        }

        i++; 

        while (isspace(json_str[i])) 
            i++;

        bool is_digit = 0;
        if (json_str[i] == '"') {
            i++;
            value_index = 0;
            while (json_str[i] != '"' && i < len) {
                value[value_index++] = json_str[i++];
            }
            value[value_index] = '\0'; 
            i++; // Пропускаем кавычку
        } else if (isdigit(json_str[i]) || json_str[i] == '-') {
            value_index = 0;
            while (isdigit(json_str[i]) || json_str[i] == '.' || json_str[i] == '-') {
                value[value_index++] = json_str[i++];
            }
            value[value_index] = '\0';
            is_digit = 1;
        } else {
            printf("ERROR: Ожидалось значение строки или числа.\n");
            return NULL;
        }


        if (PyDict_SetItem(dict,  Py_BuildValue("s", strdup(key)),  ((is_digit)?  Py_BuildValue("i", atoi(strdup(value))) : Py_BuildValue("s",strdup(value)))) < 0) {
            printf("ERROR: Failed to set item\n");
            return NULL;
        }

        while (isspace(json_str[i])) 
            i++;

        if (json_str[i] == '}') 
            break;
    
        if (json_str[i] == ',') {
            i++;
            continue;
        }

        printf("ERROR: Ожидался символ ',' или '}'\n");
        return NULL;
    }

    return dict;
}

// char* dumps(KeyValuePair *pairs, int pair_count) {
//     char *json_str = (char*)malloc(1024); 
//     int pos = 0;
//     pos += sprintf(json_str + pos, "{");
//     for (int i = 0; i < pair_count; i++) {
//         pos += sprintf(json_str + pos, "\"%s\": \"%s\"", pairs[i].key, pairs[i].value);
//         if (i < pair_count - 1) {
//             pos += sprintf(json_str + pos, ", ");
//         }
//     }
//     pos += sprintf(json_str + pos, "}");
//     return json_str;
// }

static PyObject* cjson_dumps(PyObject* self, PyObject* args) 
{
    PyObject *dict;
    if (!PyArg_ParseTuple(args, "O", &dict)) 
        return NULL;
    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a Python dictionary");
        return NULL;
    }
    PyObject *json_str = Py_BuildValue("s", "{}");  
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    while (PyDict_Next(dict, &pos, &key, &value)) {
        const char *key_str = PyUnicode_AsUTF8(key);
        const char *value_str = PyUnicode_AsUTF8(value);
        if (key_str && value_str) 
            json_str = Py_BuildValue("s", strcat(strdup(json_str), key_str));
    }
    return json_str;
}

static PyMethodDef CjsonMethods[] = {
    {"loads", cjson_loads, METH_VARARGS, "Parse JSON string into Python dict"},
    {"dumps", cjson_dumps, METH_VARARGS, "Serialize Python dict into JSON string"},
    {NULL, NULL, 0, NULL} 
};

static struct PyModuleDef cjsonmodule = {
    PyModuleDef_HEAD_INIT,
    "cjson",  
    NULL,      
    -1,      
    CjsonMethods
};

PyMODINIT_FUNC PyInit_cjson(void) {
    return PyModule_Create(&cjsonmodule); 
}
