from flask import Flask, request, send_file, jsonify
from gtts import gTTS

import os
import uuid
import ffmpeg
import whisper
import tempfile

app = Flask(__name__)


@app.route("/tts", methods=["POST"])
def tts():
    data = request.get_json()
    text = data.get("data", "") if data else ""

    print(f"tts request with data. {len(text)}")

    if not text:
        return {"error": "No text provided"}, 400

    unique_id = uuid.uuid4().hex
    mp3_path = os.path.join("output", f"{unique_id}.mp3")
    wav_path = os.path.join("output", f"{unique_id}.wav")

    # TTS 생성 (MP3)
    tts = gTTS(text=text, lang="ko")
    tts.save(mp3_path)

    # MP3 -> WAV 변환 (16bit PCM, Mono, 16000Hz)
    ffmpeg.input(mp3_path).output(wav_path, ar=16000, ac=1, sample_fmt="s16").run()

    # MP3 파일 제거
    os.remove(mp3_path)

    # 변환된 WAV 파일 반환
    response = send_file(wav_path, mimetype="audio/wav")

    @response.call_on_close
    def cleanup():
        os.remove(wav_path)

    return response


model = whisper.load_model("base")


@app.route("/transcribe", methods=["POST"])
def transcribe_audio():
    if "audio" not in request.files:
        return jsonify({"error": "No audio file provided"}), 400

    audio_file = request.files["audio"]

    # 임시 파일로 저장
    with tempfile.NamedTemporaryFile(delete=False, suffix=".wav") as temp_audio:
        audio_path = temp_audio.name
        audio_file.save(audio_path)

    try:
        result = model.transcribe(audio_path, language="ko")
        print(result)
        # return jsonify({"text": result["text"]})
        return jsonify(result)
    finally:
        # 사용 후 임시 파일 삭제
        os.remove(audio_path)


if __name__ == "__main__":
    os.makedirs("output", exist_ok=True)
    app.run(host="0.0.0.0", port=5000)
