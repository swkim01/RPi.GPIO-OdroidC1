/*
Copyright (c) 2015 Dong-Seok Lee, Seong-Woo Kim

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "c_gpio.h"
#include "common.h"
#include "py_adc.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define AdcNode0 "/sys/class/saradc/saradc_ch0"
#define AdcNode1 "/sys/class/saradc/saradc_ch1"
const char* adcFileName[2]={AdcNode0,AdcNode1};
typedef struct
{
    PyObject_HEAD
    unsigned int adcport;
    int adcfd;
} ADCObject;

// python method ADC.__init__(self, port)
static int ADC_init(ADCObject *self, PyObject *args)
{
    int port;
    int fd;
    if (!PyArg_ParseTuple(args, "i", &port))
        return -1;

    if(port < 0 || port > 1) {
        PyErr_SetString(PyExc_RuntimeError, "port is only 0 or 1");
        return -1;
    }
    if((fd=open(adcFileName[port],O_RDONLY))==-1)
    {
        PyErr_Format(PyExc_RuntimeError, "ADC Port %d is not work : %s",port,adcFileName[port]);
        return -1;
    }
    self->adcport=port;
    self->adcfd=fd;
    return 0;
}

// deallocation method
static void ADC_dealloc(ADCObject *self)
{
    close(self->adcfd);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

// python method ADC.read(self)
static PyObject *ADC_read(ADCObject *self)
{
    unsigned char value[5]={0,};
    PyObject *result;
    int v;
    lseek(self->adcfd, 0L, SEEK_SET);
    read(self->adcfd, &value[0], 4);
    v=atoi((const char *)value);
    result=Py_BuildValue("i",v);
    return result; 
}
static PyMethodDef
ADC_methods[] = {
    { "read", (PyCFunction)ADC_read, METH_NOARGS, "read ADC value" },
    {NULL, NULL, 0, NULL}
};

PyTypeObject ADCType = {
   PyVarObject_HEAD_INIT(NULL,0)
   "RPi.GPIO.ADC",            // tp_name
   sizeof(ADCObject),         // tp_basicsize
   0,                         // tp_itemsize
   (destructor)ADC_dealloc,   // tp_dealloc
   0,                         // tp_print
   0,                         // tp_getattr
   0,                         // tp_setattr
   0,                         // tp_compare
   0,                         // tp_repr
   0,                         // tp_as_number
   0,                         // tp_as_sequence
   0,                         // tp_as_mapping
   0,                         // tp_hash
   0,                         // tp_call
   0,                         // tp_str
   0,                         // tp_getattro
   0,                         // tp_setattro
   0,                         // tp_as_buffer
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flag
   "Read ADC class",    // tp_doc
   0,                         // tp_traverse
   0,                         // tp_clear
   0,                         // tp_richcompare
   0,                         // tp_weaklistoffset
   0,                         // tp_iter
   0,                         // tp_iternext
   ADC_methods,               // tp_methods
   0,                         // tp_members
   0,                         // tp_getset
   0,                         // tp_base
   0,                         // tp_dict
   0,                         // tp_descr_get
   0,                         // tp_descr_set
   0,                         // tp_dictoffset
   (initproc)ADC_init,        // tp_init
   0,                         // tp_alloc
   0,                         // tp_new
};

PyTypeObject *ADC_init_ADCType(void)
{
    ADCType.tp_new = PyType_GenericNew;
    if(PyType_Ready(&ADCType)<0)
        return NULL;
    return &ADCType;
}
