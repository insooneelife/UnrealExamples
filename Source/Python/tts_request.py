import requests

url = "http://localhost:5000/tts"
headers = {
    "Content-Type": "application/json"
}
data = {
    "data": "안녕하세요. 반갑습니다."
}

response = requests.get(url, headers=headers, json=data)

# WAV 파일 저장
if response.status_code == 200:
    with open("output.wav", "wb") as f:
        f.write(response.content)
    print("WAV 파일 저장 완료.")
else:
    print("요청 실패:", response.json())
