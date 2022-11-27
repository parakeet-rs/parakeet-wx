#!/bin/bash

set -ex

cd "$(git rev-parse --show-toplevel)"

PATH_CODE_TEMPLATE="$(realpath "${PWD}/code-template")"
PATH_OUTPUT_DIR="$(realpath "${PWD}/src/utils/json/generated")"

# generate_json_converter <ExtraImport> <ClassNameSpace> <ClassName> <item1> [item 2] ...
generate_json_converter() {
  local CONFIG_EXTRA_IMPORT="$1"
  local CONFIG_CLASS_NAMESPACE="$2"
  local CONFIG_CLASS_NAME="$3"
  local CODE_TO_JSON=""
  local CODE_FROM_JSON=""
  local LINE=""
  shift
  shift
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
    CONFIG_CLASS_NAMESPACE="${CONFIG_CLASS_NAMESPACE}" \
    envsubst <"${PATH_CODE_TEMPLATE}/ConfigConverter.h.txt" >"${PATH_OUTPUT_DIR}/${CONFIG_CLASS_NAME}Converter.h"

  CONFIG_CLASS_NAME="${CONFIG_CLASS_NAME}" \
    CONFIG_CLASS_NAMESPACE="${CONFIG_CLASS_NAMESPACE}" \
    CONFIG_EXTRA_IMPORT="${CONFIG_EXTRA_IMPORT}" \
    CODE_FROM_JSON="${CODE_FROM_JSON}" \
    CODE_TO_JSON="${CODE_TO_JSON}" \
    envsubst <"${PATH_CODE_TEMPLATE}/ConfigConverter.cpp.txt" >"${PATH_OUTPUT_DIR}/${CONFIG_CLASS_NAME}Converter.cpp"
}

# gen_decryption_json_converter_with_import <import> <ClassName> <item1> [item 2] ...
gen_decryption_json_converter_with_import() {
  local EXTRA_IMPORT="$1"
  shift

  generate_json_converter "${EXTRA_IMPORT}" "parakeet_crypto::decryption::config" "$@"
}

# gen_decryption_json_converter <ClassName> <item1> [item 2] ...
gen_decryption_json_converter() {
  gen_decryption_json_converter_with_import "" "$@"
}

gen_decryption_json_converter "KuwoConfig" "key"
gen_decryption_json_converter "KugouConfig" "slot_key_1" "v4_file_key_expansion_table" "v4_slot_key_expansion_table"
gen_decryption_json_converter "QMCConfig" "ekey_seed" "enc_v2_stage1_key" "enc_v2_stage2_key" "static_cipher_key"
gen_decryption_json_converter "NeteaseConfig" "key"
gen_decryption_json_converter "JooxConfig" "install_uuid" "salt"

generate_json_converter "" "parakeet_crypto::decryption::ximalaya" "XmlyScrambleTableParameter" "init_value" "step_value"
XIMALAYA_CONFIG_IMPORT="$(
  cat <<'EOF'
#include "utils/json/OptionalContainerConverter.hpp"
#include "XmlyScrambleTableParameterConverter.h"
EOF
)"
gen_decryption_json_converter_with_import "${XIMALAYA_CONFIG_IMPORT}" "XimalayaConfig" "x2m_content_key" "x2m_scramble_table" "x2m_scramble_table_parameters" "x3m_content_key" "x3m_scramble_table" "x3m_scramble_table_parameters"
