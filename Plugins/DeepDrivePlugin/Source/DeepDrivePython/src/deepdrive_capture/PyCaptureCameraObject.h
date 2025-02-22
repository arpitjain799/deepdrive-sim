
#pragma once

#include "Python.h"
#include "structmember.h"

#include "common/NumPyUtils.h"

#include <stdint.h>


struct PyCaptureCameraObject
{
	static int initNumPy();
	static void init(PyCaptureCameraObject *self);
	static PyObject* allocate();

	static PyArrayObject* createImage(int size, const uint8_t *data);

	PyObject_HEAD

	uint32_t			type;
	uint32_t			id;

	double				horizontal_field_of_view;
	double				aspect_ratio;


	uint32_t			capture_width;
	uint32_t			capture_height;

	PyArrayObject		*image_data;
	PyArrayObject		*depth_data;

};

static PyMemberDef PyCaptureCameraMembers[] =
{
	{"type", T_UINT, offsetof(PyCaptureCameraObject, type), 0, "Capture snapshot sequence number"}
,	{"id", T_UINT, offsetof(PyCaptureCameraObject, id), 0, "Capture snapshot sequence number"}
,	{"horizontal_field_of_view", T_DOUBLE, offsetof(PyCaptureCameraObject, horizontal_field_of_view), 0, "Horizontal field of view in radians"}
,	{"aspect_ratio", T_DOUBLE, offsetof(PyCaptureCameraObject, aspect_ratio), 0, "Aspect ratio"}
,	{"capture_width", T_UINT, offsetof(PyCaptureCameraObject, capture_width), 0, "Capture width"}
,	{"capture_height", T_UINT, offsetof(PyCaptureCameraObject, capture_height), 0, "Capture height"}
,	{"image_data", T_OBJECT_EX, offsetof(PyCaptureCameraObject, image_data), 0, "Image data"}
,	{"depth_data", T_OBJECT_EX, offsetof(PyCaptureCameraObject, depth_data), 0, "Depth data"}
,	{NULL}
};

static PyObject* PyCaptureCameraObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	return PyCaptureCameraObject::allocate();
}

static int PyCaptureCameraObject_init(PyObject *self, PyObject *args, PyObject *kwds)
{
	PyCaptureCameraObject::init(reinterpret_cast<PyCaptureCameraObject*> (self));
	return 0;
}

static PyTypeObject PyCaptureCameraType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"CaptureCamera",		//	tp name
	sizeof(PyCaptureCameraObject), 		//	tp_basicsize
	0,		//	tp_itemsize
	0,		//	tp_dealloc
	0,		//	tp_print
	0,		//	tp_getattr
	0,		//	tp_setattr
	0,		//	tp_reserved
	0,		//	tp_repr
	0,		//	tp_as_number
	0,		//	tp_as_sequence
	0,		//	tp_as_mapping
	0,		//	tp_hash
	0,		//	tp_call
	0,		//	tp_str
	0,		//	tp_getattro
	0,		//	tp_setattro
	0,		//	tp_as_buffer
	Py_TPFLAGS_DEFAULT,		//	tp_flags
	"Capture camera",		//tp_doc
	0,		//	tp_traverse
	0,		//	tp_clear
	0,		//	tp_richcompare
	0,		//	tp_weaklistoffset
	0,		//	tp_iter
	0,		//	tp_iternext
	0,		//	tp_methods
	PyCaptureCameraMembers,		//	tp_members
	0,		//	tp_getset
	0,		//	tp_base
	0,		//	tp_dict
	0,		//	tp_descr_get
	0,		//	tp_descr_set
	0,		//	tp_dictoffset
	PyCaptureCameraObject_init,		//	tp_init
	0,		//	tp_alloc
	PyCaptureCameraObject_new,		//	tp_new
};
