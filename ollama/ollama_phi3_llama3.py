from langchain_community.llms import Ollama
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser

# 初始化兩個不同的 Ollama 模型
model1 = Ollama(model='phi3')
model2 = Ollama(model='llama3')

# 定義一個自定義輸出解析器，將 "system" 替換為空字符串
class MyOutputParser(StrOutputParser):
    def parse(self, text):
        return text.replace("system", "")

# 設置第一個提示模板，用於將中文翻譯成正式的英文
prompt1 = ChatPromptTemplate.from_messages([
    ("system", "請將下面句子翻譯成正式點的英文"),
    ('user', "{ask}")
])

# 創建一個自定義輸出解析器實例
output_parser = MyOutputParser()

# 建立第一個處理鏈，包含第一個提示模板和第一個模型
chain1 = prompt1 | model1

# 用戶輸入的中文句子
input_text = "今天天氣如何"

# 執行第一個鏈並獲取回應
response1 = chain1.invoke({"ask": input_text})

# 設置第二個提示模板，用於將中文翻譯成一般的英文
prompt2 = ChatPromptTemplate.from_messages([
    ("system", "請將下面句子翻譯成英文"),
    ('user', "{ask}")
])

# 建立第二個處理鏈，包含第二個提示模板、第二個模型和自定義輸出解析器
chain2 = prompt2 | model2 | output_parser

# 使用第一個鏈的回應作為第二個鏈的輸入
response2 = chain2.invoke({"ask": response1})
print(response2)

# 如果需要再次使用第二個鏈進行翻譯，可以直接調用
new_input = "今天天氣如何"
response3 = chain2.invoke({"ask": new_input})
print(response3)
