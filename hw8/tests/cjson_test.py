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
            #"boolean": fake.boolean()
        })
    return large_json

def test_correctness(generate_large_json):
    for json_ in generate_large_json:  
        json_str = json.dumps(json_).replace("\\n", ";")    #  зачем он такое генерит...
        json_doc = json.loads(json_str)
        ujson_doc = ujson.loads(json_str)
        cjson_doc = cjson.loads(json_str)

        assert json_doc == ujson_doc == cjson_doc
        assert json_str == cjson.dumps(cjson.loads(json_str))

tms = list()

@pytest.mark.parametrize("library", ["json", "ujson", "cjson"])
def test_performance(library, generate_large_json):
    global tms
    json_str = generate_large_json

    if library == "json":
        lib = json
    elif library == "ujson":
        lib = ujson
    elif library == "cjson":
        lib = cjson

    start_time = time.time()
    for _ in range(20): 
        for json_ in generate_large_json:  
            lib.loads(json.dumps(json_).replace("\\n", ";"))
    elapsed_time = time.time() - start_time
    tms.append(str(elapsed_time) + " " + library)

    assert elapsed_time >= .1 # тест более 100 мс
#    print(f"{library}: {elapsed_time:.3f} сек")


def test_cjson_not_worst():   # не хуже всех или отстает не более чем на 5% (например)
    global tms
    tms.sort()
    assert ([1 if e.find("cjson") != -1 else 0 for e in tms].index(1) != len(tms)-1) or ((tms[-1]-tms[-2])/tms[-2] <= .05)