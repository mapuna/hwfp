%module hwfp
%{
    #include "cpuinfo.h"
    #include "boardinfo.h"
%}

%include "cpuinfo.h"
%include "boardinfo.h"

// Define the custom to_dict method for cpu_info_t, returning None for empty or "N/A" fields
%extend cpu_info_t {
    PyObject *to_dict() {
        PyObject *py_dict = PyDict_New();
        
        // Handle cpu_serial: return None if it's "N/A" or empty
        if (strlen($self->cpu_serial) == 0 || strcmp($self->cpu_serial, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "cpu_serial", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "cpu_serial", PyUnicode_FromString($self->cpu_serial));
        }

        // Handle product_uuid: return None if it's "N/A" or empty
        if (strlen($self->product_uuid) == 0 || strcmp($self->product_uuid, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "product_uuid", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "product_uuid", PyUnicode_FromString($self->product_uuid));
        }

        // Handle cpu_model: return None if it's empty
        if (strlen($self->cpu_model) == 0) {
            PyDict_SetItemString(py_dict, "cpu_model", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "cpu_model", PyUnicode_FromString($self->cpu_model));
        }

        // Handle cpu_cores
        PyDict_SetItemString(py_dict, "cpu_cores", PyLong_FromLong($self->cpu_cores));

        // Handle cache_size
        PyDict_SetItemString(py_dict, "cache_size", PyLong_FromLong($self->cache_size));

        // Handle soc_info: return None if it's "N/A" or empty
        if (strlen($self->soc_info) == 0 || strcmp($self->soc_info, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "soc_info", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "soc_info", PyUnicode_FromString($self->soc_info));
        }
        
        return py_dict;
    }
}

%extend board_info_t {
    PyObject *to_dict() {
        PyObject *py_dict = PyDict_New();
        
        if (strlen($self->board_serial) == 0 || strcmp($self->board_serial, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "board_serial", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "board_serial", PyUnicode_FromString($self->board_serial));
        }

        if (strlen($self->board_model) == 0 || strcmp($self->board_model, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "board_model", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "board_model", PyUnicode_FromString($self->board_model));
        }

        if (strlen($self->vendor) == 0) {
            PyDict_SetItemString(py_dict, "vendor", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "vendor", PyUnicode_FromString($self->vendor));
        }

        if (strlen($self->board_revision) == 0 || strcmp($self->board_revision, "N/A") == 0) {
            PyDict_SetItemString(py_dict, "board_revision", Py_None);
        } else {
            PyDict_SetItemString(py_dict, "board_revision", PyUnicode_FromString($self->board_revision));
        }

        return py_dict;
    }

}
