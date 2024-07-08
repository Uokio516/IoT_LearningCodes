# Ollama
# 這段代碼用於設置和運行 Ollama 模型 'llama3' 並與其進行交互
# 先確保安裝必要的包
# pip install langchain
# pip install -U langchain-community

from langchain_community.llms import Ollama
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser

# 初始化 Ollama 模型，使用 'llama3'
model = Ollama(model='llama3')

# 設置聊天提示模板
# 其中包含一個 system 訊息，告訴模型它是一個聊天機器人，並且要求用中文回答問題
# user 訊息模板中使用了佔位符 {ask}，該佔位符將被實際的用戶輸入替換
prompt = ChatPromptTemplate.from_messages([
    ("system", "這是一個聊天機器人。請用中文回答問題。"),
    ('user', "{ask}")
])

# 將提示模板和模型連接起來形成一個鏈
chain = prompt | model

# 用戶輸入的提示
user_prompt = "How are you?"

# 執行鏈並獲取回應，將用戶的提示作為輸入
response = chain.invoke({"ask": user_prompt})

# 打印模型的回應
print(response)

# 用戶輸入的另一個提示
user_prompt = "今天天氣如何"

# 再次執行鏈並獲取回應，將新的用戶提示作為輸入
response = chain.invoke({"ask": user_prompt})

# 打印模型的回應
print(response)
