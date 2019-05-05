#!/bin/bash

usage() { echo "Usage: ${0} [-o <target_name>] <source_name>" 1>&2; exit 1; }

while getopts ":ho:" arg; do
    case "${arg}" in
        o)
            out_path=${OPTARG}
            ;;
        h | *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${1}" ]; then
    usage
fi

in_path="${1}"

if [ -z "${out_path}" ]; then
    out_path="${in_path}.out"
fi

"slang-compiler/parser/SLang Compiler" /json "${in_file}"
"slang-compiler/slang_jtll" "${in_path}.json"
rm "${in_path}.json"
clang -o "${out_path}" app.o
rm app.o
