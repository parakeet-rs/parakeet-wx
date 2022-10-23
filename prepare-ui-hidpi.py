#!/usr/bin/env python3
import re

UI_CODE_PATH = 'um-desktop/ui/ui.cpp'

def process_code_for_hi_dpi(text):
  def addWithSizeReplace(m):
    return f'{m.group(1)} FromDIP({m.group(2)}), FromDIP({m.group(3)}), {m.group(4)}'

  def addWithBorderReplace(m):
    return f'{m.group(1)} FromDIP({m.group(2)}) {m.group(3)}'

  text = re.sub(rAddWithSize, addWithSizeReplace, text)
  text = re.sub(rAddWithBorder, addWithBorderReplace, text)
  return text

# component->Add( DIP(width), DIP(height), proportion, ...
rAddWithSize = re.compile(r'(->Add\() (\d+), (\d+), (\d+, .+?\);)')
rAddWithBorder = re.compile(r'(->Add\( .+?,) (\d+) (\);)')

if __name__ == '__main__':
  with open(UI_CODE_PATH, 'r') as f:
    file_content = f.read()
  fixed_code = process_code_for_hi_dpi(file_content)
  with open(UI_CODE_PATH, 'w') as f:
    f.write(fixed_code)
