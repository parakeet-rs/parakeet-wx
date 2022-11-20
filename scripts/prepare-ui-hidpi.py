#!/usr/bin/env python3
import os
import re

__dirname = os.path.dirname(os.path.realpath(__file__)) + '/..'

UI_CODE_PATH = f'{__dirname}/src/ui/ui.cpp'
UI_HDR_PATH = f'{__dirname}/src/ui/ui.h'

hidpi_macro = '''
/// HiDPI polyfill
#include <wx/version.h> 
#if !wxCHECK_VERSION(3, 1, 0) && !defined(FromDIP)
#define FromDIP(x) (x)
#endif
///
'''


def process_header_for_hi_dpi(text):
    if hidpi_macro not in text:
        text = text.replace('namespace', f'{hidpi_macro}\nnamespace', 1)

    return text


def process_code_for_hi_dpi(text):

    def addWithSizeReplace(m):
        return f'{m.group(1)} FromDIP({m.group(2)}), FromDIP({m.group(3)}), {m.group(4)}'

    def addWithBorderReplace(m):
        return f'{m.group(1)} FromDIP({m.group(2)}) {m.group(3)}'

    # component->Add( DIP(width), DIP(height), proportion, ...
    rAddWithSize = re.compile(r'(->Add\() (\d+), (\d+), (\d+, .+?\);)')
    text = re.sub(rAddWithSize, addWithSizeReplace, text)

    # component->Add( ..., DIP(border));
    rAddWithBorder = re.compile(r'(->Add\( .+?,) (\d+) (\);)')
    text = re.sub(rAddWithBorder, addWithBorderReplace, text)

    return text


if __name__ == '__main__':
    with open(UI_CODE_PATH, 'r', newline='\n') as f:
        file_content = f.read()
    fixed_code = process_code_for_hi_dpi(file_content)
    with open(UI_CODE_PATH, 'w', newline='\n') as f:
        f.write(fixed_code)

    with open(UI_HDR_PATH, 'r', newline='\n') as f:
        file_content = f.read()
    fixed_code = process_header_for_hi_dpi(file_content)
    with open(UI_HDR_PATH, 'w', newline='\n') as f:
        f.write(fixed_code)
