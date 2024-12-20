import json
import ujson
import cjson
import pytest
from faker import Faker
import time

@pytest.fixture(scope="module")
def generate_large_json():
    fake = Faker()
    large_json = []
    for _ in range(10000):  
        large_json.append({
            "name": fake.name(),
            "address": fake.address(),
            "email": fake.email(),
            "text": fake.text(max_nb_chars=200),
            "integer": fake.random_int(min=0, max=100000),
            "boolean": fake.boolean()
        })
    return json.dumps(large_json)

def test_correctness(generate_large_json):
    json_str = generate_large_json
    json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)

    assert json_doc == ujson_doc == cjson_doc
    assert json_str == cjson.dumps(cjson.loads(json_str))

@pytest.mark.parametrize("library", ["json", "ujson", "cjson"])
def test_performance(library, generate_large_json):
    json_str = generate_large_json

    if library == "json":
        lib = json
    elif library == "ujson":
        lib = ujson
    elif library == "cjson":
        lib = cjson

    start_time = time.time()
    for _ in range(10): 
        lib.loads(json_str)
    elapsed_time = time.time() - start_time

    print(f"{library}: {elapsed_time:.3f} сек")

    #assert elapsed_time >= 0.1
