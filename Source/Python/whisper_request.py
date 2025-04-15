import requests

# API 서버 주소
url = "http://localhost:5000/transcribe"

# 전송할 오디오 파일 경로 (예: output/audio.wav)
audio_path = "output/audio.wav"

# 파일 열고 POST 요청
with open(audio_path, "rb") as audio_file:
    files = {'audio': audio_file}
    response = requests.post(url, files=files)

# 결과 출력
if response.status_code == 200:
    result = response.json()
    print("변환된 텍스트:", result["text"])
else:
    print("오류 발생:", response.status_code, response.text)