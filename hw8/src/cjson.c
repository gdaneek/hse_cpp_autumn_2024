#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"
#define MAX_KEY_VALUE_LENGTH 1024

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

static PyObject* cjson_dumps(PyObject* self, PyObject* args) {
    PyObject *dict;
    if (!PyArg_ParseTuple(args, "O", &dict)) 
        return NULL;

    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a Python dictionary");
        return NULL;
    }

    PyObject *json_str = PyUnicode_FromString("{");

    PyObject *key, *value;
    Py_ssize_t pos = 0;
    int first = 1;  

    while (PyDict_Next(dict, &pos, &key, &value)) {
        const char *key_str = PyUnicode_AsUTF8(key);
        if (!key_str) {
            PyErr_SetString(PyExc_TypeError, "Key must be a string");
            return NULL;
        }

        const char *value_str = NULL;
        bool is_digit = 0;
        if (PyUnicode_Check(value)) {
            value_str = PyUnicode_AsUTF8(value);
        } else if (PyLong_Check(value)) {
            is_digit = 1;
            value_str = PyUnicode_AsUTF8(PyObject_Str(value));
        } else if (PyFloat_Check(value)) {
            value_str = PyUnicode_AsUTF8(PyObject_Str(value));
            is_digit = 1;
        } else {
            PyErr_SetString(PyExc_TypeError, "Value must be a string, integer, or float");
            return NULL;
        }

        if (!value_str) {
            PyErr_SetString(PyExc_TypeError, "Value conversion to string failed");
            return NULL;
        }

        if (!first) {
            PyObject *comma = PyUnicode_FromString(", ");
            json_str = PyUnicode_Concat(json_str, comma);
            Py_DECREF(comma);
        }

        PyObject *pair;
        if(!is_digit)
            pair = PyUnicode_FromFormat("\"%s\": \"%s\"", key_str, value_str);
        else 
            pair = PyUnicode_FromFormat("\"%s\": %s", key_str, value_str);
        json_str = PyUnicode_Concat(json_str, pair);
        Py_DECREF(pair);

        first = 0;
    }

    PyObject *closing_brace = PyUnicode_FromString("}");
    json_str = PyUnicode_Concat(json_str, closing_brace);
    Py_DECREF(closing_brace);

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
