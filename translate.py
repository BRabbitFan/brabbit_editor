import xlrd2
import os
import argparse

############################## define ##############################

WORK_DIR = os.path.dirname(__file__) + "/"                          # 工作目录

DEFAULT_FILE_NAME  = "translate"

EXCEL_FILE_PATH    = WORK_DIR + "./" + DEFAULT_FILE_NAME + ".xlsx"  # 存放翻译文本的 excel 文件路径
EXCEL_SOURCE_COL   = 0                                              # 原文所在列
EXCEL_COMMENT_COL  = 1                                              # 注释所在列
EXCEL_I18N_COL_BEG = 2                                              # i18n 标签首列
EXCEL_I18N_ROW     = 0                                              # i18n 标签所在行
EXCEL_TEXT_ROW_BEG = 1                                              # 译文首行

XML_DIR_PATH       = WORK_DIR + "./"                                # .xml 文件输出目录
JSON_DIR_PATH      = WORK_DIR + "./"                                # .json 文件输出目录
INI_DIR_PATH       = WORK_DIR + "./"                                # .ini 文件输出目录
QML_FILE_PATH      = WORK_DIR + "./" + DEFAULT_FILE_NAME + ".qml"   # .qml 文件输出路径
HPP_FILE_PATH      = WORK_DIR + "./" + DEFAULT_FILE_NAME + ".hpp"   # .hpp 文件输出路径

############################## define[end] ##############################

class Text:
  # each Text object save one text's translation about one local

  source      = ""
  comment     = ""
  translation = ""
  i18n        = ""

  def __init__(self, source      = "",
                     comment     = "",
                     translation = "",
                     i18n        = "") -> None:
    self.source      = source
    self.comment     = comment
    self.translation = translation
    self.i18n        = i18n

  def __str__(self) -> str:
    string = ""
    string += "( "
    string +=   "source:"      + self.source      + ", "
    string +=   "comment:"     + self.comment     + ", "
    string +=   "translation:" + self.translation + ", "
    string +=   "i18n:"        + self.i18n
    string += " )"
    return string

  def vaild(self):
    vaild = True
    vaild = vaild and len(self.source)      != 0
    # vaild = vaild and len(self.translation) != 0
    vaild = vaild and len(self.i18n)        != 0
    return vaild

# i18n_text_list_map = {
#   {
#     "en_US", [
#       Text,
#       Text,
#       ...
#     ], 
#   },
#   {
#     "zh_CN", [
#       ...
#     ]
#   }
# }
# 
# text_list = i18n_text_list_map[i18n]
#
# i18n_describe_map = {
#   "en_US": "English(United States)",
#   "zh_CN": "简体中文(中国大陆)",
#   ...
# }
# 
# describe = i18n_describe_map[i18n]

def dump_i18n_text_list_map(i18n_text_list_map):
  for i18n in i18n_text_list_map:
    print(i18n)
    text_list = i18n_text_list_map[i18n]
    for text in text_list:
      print(text)

def scan_excel_file(file_path):
  # return: i18n_text_list_map, i18n_describe_map
  i18n_text_list_map = {}
  i18n_describe_map = {}

  print("scan .xlsx file started")

  excel_file = xlrd2.open_workbook(file_path)

  sheet_col_i18n_map = {} # i18n = sheet_col_i18n_map[sheet][col]

  # analysis each sheet and init the i18n_text_list_map struct
  for excel_sheet in excel_file.sheets():
    sheet_col_i18n_map[excel_sheet.name] = {}

    # analysis local row and record to local_list
    i18n_row_values = excel_sheet.row_values(EXCEL_I18N_ROW)
    for col_index in range(EXCEL_I18N_COL_BEG, excel_sheet.ncols):
      i18n, describe = i18n_row_values[col_index].split("/", 1)

      sheet_col_i18n_map[excel_sheet.name][col_index] = i18n

      if not i18n_text_list_map.get(i18n, False):
        i18n_text_list_map[i18n] = []

      if not i18n_describe_map.get(i18n, False):
        i18n_describe_map[i18n] = describe

  # analysis each sheet and merge their data into i18n_text_list_map
  for excel_sheet in excel_file.sheets():
    print("Scanning sheet \"" + excel_sheet.name + "\" ...")

    # # analysis local row and init col_i18n_map in this sheet
    # col_i18n_map = {}
    # i18n_row_values = excel_sheet.row_values(EXCEL_I18N_ROW)
    # for col_index in range(EXCEL_I18N_COL_BEG, excel_sheet.ncols):
    #   col_i18n_map[col_index] = i18n_row_values[col_index].split("/", 1)[0]

    # analysis each text row to record each text
    for row_index in range(EXCEL_TEXT_ROW_BEG, excel_sheet.nrows):
      row_values = excel_sheet.row_values(row_index)

      for col_index in range(EXCEL_I18N_COL_BEG, excel_sheet.ncols):
        i18n      = sheet_col_i18n_map[excel_sheet.name][col_index]
        text_list = i18n_text_list_map[i18n]

        text_list.append(Text(
          source      = row_values[EXCEL_SOURCE_COL],
          comment     = row_values[EXCEL_COMMENT_COL],
          translation = row_values[col_index],
          i18n        = i18n,
        ))

  print("scan .xlsx file finished")

  return i18n_text_list_map, i18n_describe_map

def create_xml_file(i18n_text_list_map, i18n_describe_map, output_dir_path = XML_DIR_PATH):
  # return: .xml file path list (absolute path)
  file_path_list = []

  print("create .xml file started")

  # create .xml file for each local
  for i18n in i18n_text_list_map:
    file_name = i18n + ".xml"
    file_path = output_dir_path + file_name
    file_path_list.append(file_path)

    print("Updating \"" + file_path + "\" ...")

    # analysis i18n_text_list_map to create .xml file data

    file_data = ""
    file_data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + "\n"

    file_data += "<root>" + "\n"

    file_data += "\t<head "
    file_data += "i18n=\"" + i18n + "\" "
    file_data += "describe=\"" + i18n_describe_map[i18n] + "\" "
    file_data += "/>" + "\n"

    file_data += "\t<body>" + "\n"
    
    text_list = i18n_text_list_map[i18n]
    for text in text_list:
      if not text.vaild():
        continue

      file_data += "\t\t<text "
      file_data += "source=\"" + text.source + "\" "
      file_data += "translation=\"" + text.translation + "\" "
      file_data += "comment=\"" + text.comment + "\""
      file_data += "/>" + "\n"

    file_data += "\t</body>" + "\n"
        
    file_data += "</root>" + "\n"

    # write .xml file data

    with open(file_path, "w", encoding="utf-8") as file:
      file.write(file_data)

  print("create .xml file finished")
  
  return file_path_list

def create_json_file(i18n_text_list_map, i18n_describe_map, output_dir_path = JSON_DIR_PATH):
  # return: .json file path list (absolute path)
  file_path_list = []

  print("create .json file started")

  # create .json file for each local
  for i18n in i18n_text_list_map:
    file_name = i18n + ".json"
    file_path = output_dir_path + file_name
    file_path_list.append(file_path)

    print("Updating \"" + file_path + "\" ...")


    # analysis i18n_text_list_map to create .json file data

    file_data = ""
    file_data += "{\n"

    file_data += "\t\"head\": {\n"
    file_data += "\t\t\"i18n\": \"" + i18n + "\",\n"
    file_data += "\t\t\"describe\": \"" + i18n_describe_map[i18n] + "\"\n"
    file_data += "\t},\n"

    file_data += "\t\"body\": {" + "\n"
    
    text_list = i18n_text_list_map[i18n]
    for text in text_list:
      if not text.vaild():
        continue

      file_data += "\t\t\"" + text.source + "\": \"" + text.translation + "\","
      file_data += "  \"//" + text.source + "\": \"" + text.comment + "\","
      file_data += "\n"
    
    file_data = file_data[:len(file_data) - 2]
    file_data += "\n"
    file_data += "\t}\n"
        
    file_data += "}\n"

    # write .json file data

    with open(file_path, "w", encoding="utf-8") as file:
      file.write(file_data)

  print("create .json file finished")
  
  return file_path_list

def create_ini_file(i18n_text_list_map, i18n_describe_map, output_dir_path = INI_DIR_PATH):
  # return: .ini file path list (absolute path)
  file_path_list = []

  print("create .ini file started")

  # create .ini file for each local
  for i18n in i18n_text_list_map:
    file_name = i18n + ".ini"
    file_path = output_dir_path + file_name
    file_path_list.append(file_path)

    print("Updating \"" + file_path + "\" ...")

    # analysis i18n_text_list_map to create .ini file data

    file_data = ""
    file_data += "[head]\n"
    file_data += "i18n=" + i18n + "\n"
    file_data += "describe=" + i18n_describe_map[i18n] + "\n"
    file_data += "\n"

    file_data += "[body]\n"
    
    text_list = i18n_text_list_map[i18n]
    for text in text_list:
      if not text.vaild():
        continue

      file_data += text.source + "=" + text.translation + "  ; " + text.comment + "\n"

    # write .ini file data

    with open(file_path, "w", encoding="utf-8") as file:
      file.write(file_data)

  print("create .ini file finished")
  
  return file_path_list

def create_qml_file(i18n_text_list_map, output_dir_path = QML_FILE_PATH):
  # return: .qml file path
  file_path = output_dir_path

  print("create .qml file started")

  print("Updating \"" + file_path + "\" ...")

  file_data = ""
  file_data += "pragma Singleton\n"
  file_data += "\n"
  file_data += "import QtQml 2.13\n"
  file_data += "\n"
  file_data += "QtObject {\n"
  file_data += "\n"
  file_data += "  function load(source) { return qsTr(source) }\n"
  file_data += "\n"

  for i18n in i18n_text_list_map:

    text_list = i18n_text_list_map[i18n]
    for text in text_list:
      file_data += "  "
      file_data += "readonly property string " + text.source
      file_data += ": "
      file_data += "load(\"" + text.source + "\")"
      file_data += "  // " + text.comment + "\n"

    break

  file_data += "\n"
  file_data += "}\n"

  with open(file_path, "w", encoding="utf-8") as file:
    file.write(file_data)

  print("create .qml file finished")

  return file_path

def create_hpp_file(i18n_text_list_map, output_dir_path = HPP_FILE_PATH):
  # return: .hpp file path
  file_path = output_dir_path

  print("create .hpp file started")

  print("Updating \"" + file_path + "\" ...")

  file_data = ""
  file_data += "#pragma once\n"
  file_data += "\n"
  file_data += "#include <set>\n"
  file_data += "#include <functional>\n"
  file_data += "\n"
  file_data += "#include <QCoreApplication>\n"
  file_data += "\n"
  file_data += "namespace br {\n"
  file_data += "\n"
  file_data += "namespace tr {\n"
  file_data += "\n"
  file_data += "inline QString load(const char* source) { return QObject::tr(source); }\n"
  file_data += "\n"

  for i18n in i18n_text_list_map:
    text_list = i18n_text_list_map[i18n]
    
    for text in text_list:
      file_data += "inline QString " + text.source + "()"
      file_data += " { return load(\"" + text.source + "\"); }"
      file_data += "  // " + text.comment + "\n"

    break

  file_data += "\n"
  file_data += "}\n"
  file_data += "\n"
  file_data += "}\n"

  with open(file_path, "w", encoding="utf-8") as file:
    file.write(file_data)

  print("create .hpp file finished")

  return file_path

def main():
  parser = argparse.ArgumentParser(description="scan the excel file to create script file: [xml, json, ini], and create code file: [cpp, qml]",
                                   epilog="-- by BRabbit")
  parser.add_argument("-e", dest="excel_file_path", type=str, default=EXCEL_FILE_PATH, help="input file path of excel file, default with: " + EXCEL_FILE_PATH)

  parser.add_argument("-x", dest="output_xml", type=bool, default=False, help="is need to output xml file, default with: " + str(False))
  parser.add_argument("-xp", dest="xml_dir_path", type=str, default=XML_DIR_PATH, help="output dir path of xml file, default with: " + XML_DIR_PATH)
  
  parser.add_argument("-j", dest="output_json", type=bool, default=False, help="is need to output json file, default with: " + str(False))
  parser.add_argument("-jp", dest="json_dir_path", type=str, default=JSON_DIR_PATH, help="output dir path of json file, default with: " + JSON_DIR_PATH)
  
  parser.add_argument("-i", dest="output_ini", type=bool, default=False, help="is need to output ini file, default with: " + str(False))
  parser.add_argument("-ip", dest="ini_dir_path", type=str, default=INI_DIR_PATH, help="output dir path of ini file, default with: " + INI_DIR_PATH)

  parser.add_argument("-q", dest="output_qml", type=bool, default=False, help="is need to output qml file, default with: " + str(False))
  parser.add_argument("-qp", dest="qml_file_path", type=str, default=QML_FILE_PATH, help="output file path of qml file, default with: " + QML_FILE_PATH)

  parser.add_argument("-c", dest="output_cpp", type=bool, default=False, help="is need to output c++ file, default with: " + str(False))
  parser.add_argument("-cp", dest="cpp_file_path", type=str, default=HPP_FILE_PATH, help="output file path of c++ file, default with: " + HPP_FILE_PATH)

  parser.add_argument("-all", dest="output_all", type=bool, default=False, help="output all type of file" + str(False))

  args = parser.parse_args()

  i18n_text_list_map, i18n_describe_map = scan_excel_file(args.excel_file_path)
  # dump_i18n_text_list_map(i18n_text_list_map)

  if args.output_xml or args.output_all:
    xml_file_path_list = create_xml_file(i18n_text_list_map, i18n_describe_map, args.xml_dir_path)

  if args.output_json or args.output_all:
    json_file_path_list = create_json_file(i18n_text_list_map, i18n_describe_map, args.json_dir_path)
  
  if args.output_ini or args.output_all:
    ini_file_path_list = create_ini_file(i18n_text_list_map, i18n_describe_map, args.ini_dir_path)
  
  if args.output_qml or args.output_all:
    qml_file_path = create_qml_file(i18n_text_list_map, args.qml_file_path)
  
  if args.output_cpp or args.output_all:
    hpp_file_path = create_hpp_file(i18n_text_list_map, args.cpp_file_path)

if __name__ == "__main__":
  main()
