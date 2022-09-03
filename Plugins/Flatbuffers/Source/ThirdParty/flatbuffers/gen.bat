flatc -c -o generated -I . data/monster.fbs
::flatc --python -o ../localserve/python -I . ../localserve/fbs/response.fbs ../localserve/fbs/stream.fbs