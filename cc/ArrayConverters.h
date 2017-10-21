#include "TypeConverters.h"
#include "InstanceConverter.h"

#ifndef __FF_ARRAYCONVERTER_H__
#define __FF_ARRAYCONVERTER_H__

template <class Converter, class T, class CastType = T>
class ArrayConverterType {
public:
	static const char* getTypeName() {
		return "array";
	}

  static bool unwrap(std::vector<CastType>* vec, v8::Local<v8::Value> jsVal) {
		if (!jsVal->IsArray()) {
			return true;
		}

		v8::Local<v8::Array> jsArr = v8::Local<v8::Array>::Cast(jsVal);
    for (int i = 0; i < jsArr->Length(); i++) {
      if (!Converter::assertType(jsArr->Get(i))) {
				Nan::ThrowError(
					Nan::New(
						std::string("expected array element at index ")
						+ std::to_string(i)
						+ std::string(" to be of type ")
						+ std::string(Converter::getTypeName())
					).ToLocalChecked()
        );
        return true;
      }

			CastType obj = (CastType)Converter::unwrap(jsArr->Get(i));
      vec->push_back(obj);
    }
		return false;
	}

	static v8::Local<v8::Value> wrap(std::vector<CastType> vec) {
		v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(vec.size());
		for (int i = 0; i < jsArr->Length(); i++) {
			jsArr->Set(i, Converter::wrap(vec.at(i)));
		}
		return jsArr;
	}
};

class BoolArrayConverter : public AbstractConverter<ArrayConverterType<BoolConverter, bool>, std::vector<bool>> {};
class DoubleArrayConverter : public AbstractConverter<ArrayConverterType<DoubleConverter, double>, std::vector<double>> {};
class FloatArrayConverter : public AbstractConverter<ArrayConverterType<FloatConverter, float>, std::vector<float>> {};
class UintArrayConverter : public AbstractConverter<ArrayConverterType<UintConverter, uint>, std::vector<uint>> {};
class IntArrayConverter : public AbstractConverter<ArrayConverterType<IntConverter, int>, std::vector<int>> {};

template <class Clazz, class T, class CastType = T>
class ObjectArrayConverter : public AbstractConverter<ArrayConverterType<InstanceConverterType<Clazz, CastType>, T, CastType>, std::vector<CastType>> {};

#endif