/****************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
*****************************************************************************/

%template(TgsImageUnsignedChar) Tgs::Image<unsigned char>;

%fragment("TgsImageUtilities", "header", fragment="NumPy_Fragments")
{
  namespace Tgs
  {
    /** 
     * Given a PyObject, return a Tgs::Image
     */
    template<class _T> Image<_T>* convertPyObjectToImage(PyObject* obj)
    {
      void* vp;
      Tgs::Image<_T>* result = 0;
      // try to convert from a Tgs::Image
      %#ifdef _WIN32
        int res = SWIG_ConvertPtr(obj, &vp, SWIGTYPE_p_Tgs__ImageT_unsigned_char_t, 0 |  0 );
      %#else
        int res = SWIG_ConvertPtr(obj, &vp, SWIGTYPE_p_Tgs__ImageTunsigned_char_t, 0 |  0 );
      %#endif
      Tgs::Image<_T>* from = reinterpret_cast< Tgs::Image<_T>* >(vp);
      if (SWIG_CheckState(res) && from != 0)
      {
        result = from;
      }
      // well that didn't work, try converting from a NumPy Array
      else
      {
        int isNewObject = 0;

        // try converting from double
        PyArrayObject* array1 = obj_to_array_contiguous_allow_conversion(obj, NPY_DOUBLE, 
          &isNewObject);
        if (array1)          
        {
          require_dimensions(array1, 2);
          //std::cout << array1->dimensions[1] << ", " << array1->dimensions[0] << std::endl;
          result = new Tgs::Image<_T>(array1->dimensions[1], array1->dimensions[0]);
          res = SWIG_NEWOBJMASK;
          for (int i = 0; i < result->getPixelCount(); i++)
          {
            result->getData()[i] = (_T)(((double*)(array1->data))[i]);
          }
          Py_DECREF(array1);
          return result;
        }
        
        // try converting from float
        array1 = obj_to_array_contiguous_allow_conversion(obj, NPY_FLOAT, 
          &isNewObject);
        if (array1)          
        {
          require_dimensions(array1, 2);
          result = new Tgs::Image<_T>(array1->dimensions[1], array1->dimensions[0]);
          res = SWIG_NEWOBJMASK;
          for (int i = 0; i < result->getPixelCount(); i++)
          {
            result->getData()[i] = (_T)(((float*)(array1->data))[i]);
          }
          Py_DECREF(array1);
          return result;
        }
        
        // try converting from int
        array1 = obj_to_array_contiguous_allow_conversion(obj, NPY_INT, &isNewObject);
        if (array1)          
        {
          require_dimensions(array1, 2);
          result = new Tgs::Image<_T>(array1->dimensions[1], array1->dimensions[0]);
          res = SWIG_NEWOBJMASK;
          for (int i = 0; i < result->getPixelCount(); i++)
          {
            result->getData()[i] = (_T)(((int*)(array1->data))[i]);
          }
          Py_DECREF(array1);
          return result;
        }
        
        if (result == 0) 
        {
          PyErr_SetString(PyExc_RuntimeError, "Could not convert to a Tgs::Image");
        }
      }
      return result;
    }
  }
}

namespace Tgs
{
  /**
   * unsigned char
   */
  %typemap(out, fragment="TgsImageUtilities") Image<unsigned char> {
    int dims[2];
    dims[1] = $1.getWidth();
    dims[0] = $1.getHeight();
	  $result = PyArray_FromDims(2, dims, PyArray_UBYTE);
    unsigned char* data = (unsigned char*)PyArray_DATA(resultobj);
    for (int i = 0; i < $1.getPixelCount(); i++)
    {
      data[i] = $1.getData()[i];
    }
  }
  
  %typemap(in, fragment="TgsImageUtilities") const std::vector< const Tgs::Image<unsigned char>* >& {
    if (PyList_Check($input))
    {
      $1 = new std::vector< const Tgs::Image<unsigned char>* >();
      for (int i = 0; i < PyList_Size($input); i++)
      {
        Tgs::Image<unsigned char>* image = Tgs::convertPyObjectToImage<unsigned char>(
          PyList_GetItem($input, i));
        if (image)
        {
          $1->push_back(image);
        }
        else
        {
          delete $1;
          PyErr_SetString(PyExc_RuntimeError, "One or more elements couldn't be converted to "
            "Tgs::Images.");
          SWIG_fail;
        }
      }
    }
    else
    {
      PyErr_SetString(PyExc_RuntimeError, "Must pass a list of Tgs::Images or equivalent.");
      SWIG_fail;
    }
  }

  %typemap(freearg, fragment="TgsImageUtilities") std::vector< const Tgs::Image<unsigned char>* >& {
    for (std::vector< const Tgs::Image<unsigned char>* >::const_iterator it = $1->begin(); 
      it != $1->end(); it++)
    {
      delete *it;
    }
    delete $1;
  }

  %typemap(in, fragment="TgsImageUtilities") Image<unsigned char>& {
    $1 = Tgs::convertPyObjectToImage<unsigned char>($input);
    if ($1 == 0) SWIG_fail;
  }

  %typemap(freearg, fragment="TgsImageUtilities") Image<unsigned char>& {
    delete $1;
  }

  /**
   * float
   */
  %typemap(out, fragment="TgsImageUtilities") Image<float> {
    int dims[2];
    dims[1] = $1.getWidth();
    dims[0] = $1.getHeight();
	  $result = PyArray_FromDims(2, dims, PyArray_FLOAT);
    float* data = (float*)PyArray_DATA(resultobj);
    for (int i = 0; i < $1.getPixelCount(); i++)
    {
      data[i] = $1.getData()[i];
    }
  }
  
  %typemap(in, fragment="TgsImageUtilities") Image<float>& {
    $1 = Tgs::convertPyObjectToImage<float>($input);
    if ($1 == 0) SWIG_fail;
  }

  %typemap(freearg, fragment="TgsImageUtilities") Image<float>& {
    delete $1;
  }
}


