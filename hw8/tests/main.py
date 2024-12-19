#! /usr/bin/env python3

import json
import cjson

def main():
    json_str = '{"hello": 10, "world": "value"}'
    json_doc = json.loads(json_str)
    cjson_doc = cjson.loads(json_str)
    assert json_doc  == cjson_doc
    #assert json_str == cjson.dumps(cjson.loads(json_str))

if __name__ == "__main__":
    main() 
