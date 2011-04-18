//	I used to use the macro form of these 2 a lot,
//	so now in the newfangled days of oop and c++ i thought it may be nice to have templated versions:)

//	safe delete template

template <typename T>	void safeDelete(T *&ptr)
{
	delete ptr;
	ptr = NULL;
}

template <typename T>	void safeDeleteArray(T *&ptr)
{
	delete[] ptr;
	ptr = NULL;
}

// and heres the legacy versions (im still fond of these)
#define SAFE_DELETE(p)				{	delete (p);		(p) = NULL;	}
#define SAFE_DELETE_ARRAY(p)		{	delete[] (p);	(p) = NULL;	}
