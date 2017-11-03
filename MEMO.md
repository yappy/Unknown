# Memo

## vorbis tools
(Linux 向け)

wav を以下のツールで変換すれば事足りると思う(多分)
```
$ apt-get install vorbis-tools

/usr/bin/ogg123
/usr/bin/oggdec
/usr/bin/oggenc
/usr/bin/ogginfo
/usr/bin/vcut
/usr/bin/vorbiscomment
/usr/bin/vorbistagedit
```

## SDL_mixer
2.0.2 は全プラットフォーム共通で初期化が必ず失敗する致命傷がある。
