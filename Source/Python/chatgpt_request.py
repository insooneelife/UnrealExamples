import requests
import json
import os
from dotenv import load_dotenv

# .env 파일에서 환경 변수 로드
load_dotenv()

# API 키 설정
api_key = os.getenv("OPENAI_API_KEY")

print(api_key)

# API 엔드포인트
url = "https://api.openai.com/v1/chat/completions"

# 요청 헤더
headers = {"Content-Type": "application/json", "Authorization": f"Bearer {api_key}"}


def send_chat_request(prompt, model="gpt-3.5-turbo"):
    """
    ChatGPT API에 요청을 보내고 응답을 받는 함수

    Args:
        prompt (str): 사용자 입력 텍스트
        model (str): 사용할 모델 (기본값: gpt-3.5-turbo)

    Returns:
        str: ChatGPT의 응답 텍스트
    """
    # 요청 데이터
    data = {
        "model": model,
        "messages": [{"role": "user", "content": prompt}],
        "temperature": 0.7,
        "max_tokens": 1000,
    }

    try:
        # API 요청 보내기
        response = requests.post(url, headers=headers, data=json.dumps(data))

        # 응답 확인
        if response.status_code == 200:
            result = response.json()
            return result["choices"][0]["message"]["content"]
        else:
            return f"오류 발생: {response.status_code} - {response.text}"

    except Exception as e:
        return f"요청 중 오류 발생: {str(e)}"


def send_chat_request_with_history(messages, model="gpt-3.5-turbo"):
    """
    대화 기록을 포함하여 ChatGPT API에 요청을 보내는 함수

    Args:
        messages (list): 대화 기록 리스트 (각 항목은 role과 content를 포함하는 딕셔너리)
        model (str): 사용할 모델 (기본값: gpt-3.5-turbo)

    Returns:
        str: ChatGPT의 응답 텍스트
    """
    # 요청 데이터
    data = {
        "model": model,
        "messages": messages,
        "temperature": 0.7,
        "max_tokens": 1000,
    }

    try:
        # API 요청 보내기
        response = requests.post(url, headers=headers, data=json.dumps(data))

        # 응답 확인
        if response.status_code == 200:
            result = response.json()
            return result["choices"][0]["message"]["content"]
        else:
            return f"오류 발생: {response.status_code} - {response.text}"

    except Exception as e:
        return f"요청 중 오류 발생: {str(e)}"


def main():
    # 간단한 예제
    print("=== ChatGPT API 예제 ===")

    # 단일 메시지 요청
    prompt = "안녕하세요, 자기소개를 해주세요."
    print(f"\n질문: {prompt}")
    response = send_chat_request(prompt)
    print(f"응답: {response}")

    # 대화 기록을 포함한 요청
    print("\n=== 대화 기록을 포함한 요청 ===")
    conversation = [
        {"role": "system", "content": "당신은 친절한 AI 비서입니다."},
        {"role": "user", "content": "안녕하세요, 자기소개를 해주세요."},
        {
            "role": "assistant",
            "content": "안녕하세요! 저는 AI 비서입니다. 어떤 도움이 필요하신가요?",
        },
        {"role": "user", "content": "오늘 날씨는 어때요?"},
    ]

    response = send_chat_request_with_history(conversation)
    print(f"응답: {response}")


if __name__ == "__main__":
    main()
