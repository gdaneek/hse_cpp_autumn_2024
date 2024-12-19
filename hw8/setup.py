from setuptools import setup, Extension

module = Extension("cjson", sources=["./src/cjson.c"])

setup(
    name="cjson",
    version="1.0",
    description="Custom JSON parser and serializer",
    ext_modules=[module],
)
 
