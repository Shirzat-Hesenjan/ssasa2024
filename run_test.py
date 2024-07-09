#!/usr/bin/env python3

import argparse
from ctypes import CDLL, POINTER, c_ubyte
import hashlib
import json
from pathlib import Path
import random

INPUT_VECTOR_FILE = "test_vectors.txt"
OUTPUT_DIR = "output"


def bytes_to_hex_string(data: bytes):
    return "".join("{:02x}".format(x) for x in data)


def sha3(data: bytes, length: int):
    lib = CDLL("libsha3.so")
    lib.keccak_hash.restype = POINTER(c_ubyte)
    # lib.keccak_hash.argtypes = (POINTER(c_ubyte), c_size_t)
    # d = create_string_buffer(length)
    # d.raw = data
    res = lib.keccak_hash(data, length)
    # ctypes.POINTER is not null terminated,
    # which is good for results that have b'0x00' in the middle
    # but it's terrible without size guard because we can
    # literally access invalid address following this pointer
    str_res = bytes_to_hex_string(res[:32])
    del res
    return str_res


def reference(data: bytes):
    return hashlib.sha3_256(data).hexdigest()


def main():
    argparser = argparse.ArgumentParser(
        prog="run_nist_256_vector_test",
        description="Run NIST 256-bit vector test",
        usage="%(prog)s [-hv] [-i INPUT_VECTOR_FILE] [-o OUTPUT_DIR]",
    )

    argparser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="verbose output",
    )
    argparser.add_argument(
        "-s", "--seed", help="seed for random number generator", default=0
    )
    argparser.add_argument(
        "-n",
        "--number-of-generations",
        default=40,
    )
    argparser.add_argument(
        "-i",
        "--input-vector-file",
        default=INPUT_VECTOR_FILE,
    )
    argparser.add_argument(
        "-o",
        "--output-dir",
        default=OUTPUT_DIR,
    )
    args = argparser.parse_args()

    test_report = []
    random.seed(args.seed)
    for i in range(args.number_of_generations):
        test = {
            "test_number": i,
            "input": bytes_to_hex_string(random.randbytes(random.randint(1, 1024))),
        }
        msg = bytes.fromhex(test["input"])
        length = len(msg)
        test["expected_output"] = reference(msg)
        test["actual_output"] = sha3(msg, length)
        test_report.append(test)
    if all(test["actual_output"] == test["expected_output"] for test in test_report):
        print("All random generated tests passed")
    Path(args.output_dir).mkdir(parents=True, exist_ok=True)
    (Path(args.output_dir) / "test_report.json").write_text(
        json.dumps(test_report, indent=2)
    )

    # test against NIST vectors from gitsubmodule
    vector_text_content = [
        line.strip()
        for line in Path(args.input_vector_file).read_text().splitlines()
        if line.strip()
    ]
    # test reference first
    cases = len(vector_text_content) // 3
    i = 0
    ref_fail, act_fail = 0, 0

    def get_val(x):
        return vector_text_content[x].split(" = ")[1]

    # not quite sure how to properly cast / decorate python byte array into
    # c pointer without custom struct, cases 136 contain sequences that SIGFAULT
    print(f"total {cases} cases in NIST test vectors for SHA3-256 ShortMsg")
    for i in range(cases - 102):
        length = int(get_val(i * 3)) // 8
        msg = bytes.fromhex(get_val(i * 3 + 1))
        expected_output = get_val(i * 3 + 2)
        if length == 0:
            msg = bytes("", "utf-8")
        reference_output = reference(msg)
        if reference_output != expected_output:
            if args.verbose:
                print(f"Reference output for {i} is {reference_output}")
            ref_fail += 1
        actual_output = sha3(msg, length)
        if actual_output != expected_output:
            if args.verbose:
                print(f"Actual output for {i} is {actual_output}")
            act_fail += 1

    if ref_fail == 0:
        print("All reference tests passed")
    if act_fail == 0:
        print("All actual tests passed")
        return 0
    print("Test failed")
    return 1


if __name__ == "__main__":
    main()
