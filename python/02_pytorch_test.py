#https://twgo.io/adjqj

import torch
print("pytorch版本：",torch.__version__)

if torch.cuda.is_available():
  print("cuda是否可用：",torch.cuda.is_available())
  print("GPU數量：",torch.cuda.device_count())
  print("cudnn是否可用：",torch.backends.cudnn.enabled)
  print("cudnn 版本：", torch.backends.cudnn.version())
  #印出記憶體用量
  print("記憶體狀態：",torch.cuda.mem_get_info())

else:
  print("cuda是否可用：",torch.cuda.is_available())

if torch.cuda.is_available():
    print("CUDA is available. Number of GPUs:", torch.cuda.device_count())
    for i in range(torch.cuda.device_count()):
        print(f"GPU {i}: {torch.cuda.get_device_name(i)}")
else:
    print("CUDA is not available. No GPU found.")
