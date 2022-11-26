#!/bin/bash

set -e

cd "$(git rev-parse --show-toplevel)"

PATH_CODE_TEMPLATE="$(realpath "${PWD}/code-template")"
PATH_OUTPUT_DIR="$(realpath "${PWD}/src/config/ConfigConverters")"

generate_config() {
  # generate_config <ClassName> <item1> [item 2] ...
  local CONFIG_CLASS_NAME="$1"
  local CODE_TO_JSON=""
  local CODE_FROM_JSON=""
  local LINE=""
  shift

  while [ $# -gt 0 ]; do
    LINE="$(printf '\n  j["%s"] = data.%s;' "$1" "$1")"
    CODE_TO_JSON="${CODE_TO_JSON}${LINE}"
    LINE="$(printf '\n  data.%s = j.value("%s", def.%s);' "$1" "$1" "$1")"
    CODE_FROM_JSON="${CODE_FROM_JSON}${LINE}"
    shift
  done

  CODE_TO_JSON="${CODE_TO_JSON:3}"
  CODE_FROM_JSON="${CODE_FROM_JSON:3}"

  CONFIG_CLASS_NAME="${CONFIG_CLASS_NAME}" \
    envsubst <"${PATH_CODE_TEMPLATE}/ConfigConverter.h.txt" >"${PATH_OUTPUT_DIR}/${CONFIG_CLASS_NAME}Converter.h"

  CONFIG_CLASS_NAME="${CONFIG_CLASS_NAME}" \
    CODE_FROM_JSON="${CODE_FROM_JSON}" \
    CODE_TO_JSON="${CODE_TO_JSON}" \
    envsubst <"${PATH_CODE_TEMPLATE}/ConfigConverter.cpp.txt" >"${PATH_OUTPUT_DIR}/${CONFIG_CLASS_NAME}Converter.cpp"
}

generate_config "KuwoConfig" "key"
generate_config "KugouConfig" "slot_key_1" "v4_file_key_expansion_table" "v4_slot_key_expansion_table"
generate_config "QMCConfig" "ekey_seed" "enc_v2_stage1_key" "enc_v2_stage2_key" "static_cipher_key"
