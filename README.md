# submissions
## HAL研究所1day仕事体験提出作品
<div style="text-align: right;">
群馬大学情報学部情報学科<br>3年<br>北川 太一
<br></div>

### 以下リポジトリ内のファイルの説明
- Kirby.exe
ゲームの実行ファイル<br><br>

- ProgrammerSheet
プログラマー 作品提出シートを保存<br><br>

- Explanation.md
今回の製作作品の説明

- execute_movie.txt
作品を確認できる動画(YouTube上)をリンクにて記載
[ここ](https://youtu.be/T7SsR7F00gw)でも確認可能<br><br>

- example
プログラムで用いる画像がすべて含まれているので削除は行わないでください。
また、画像の引用元はこちらのREADMEの一番下に記載<br><br>

以下その他の情報
<br>

### ゲームの遊び方

#### タイトル画面

最初はこの画面が表示されます。
何らかのキーを押すかマウスの左クリックで次のチュートリアル画面に画面が切り替わります。

#### チュートリアル画面

タイトル画面と同様にして何らかのキーを押すかマウスの左クリックで次のゲーム画面に切り替わります。

#### ゲーム画面

ここでゲームが始まります。

##### 基本ルール

&emsp;ブロックは毎ターン上から2個ペアで落ちてきます。それをうまく配置してブロックを消してください。<br>
&emsp;ブロックが盤面の一番上の列の真ん中2マスのどちらかに配置されて、消すことができなかったらその時点でゲームオーバーとなります。<br>
&emsp;ブロックには**仲間ブロック**4種類・星ブロック・爆弾ブロックがあります。**仲間ブロック**には(リック・クー・カイン・グーイ)の4種類があります。同じ**仲間ブロック**間にグーイ以外の別の**仲間ブロック**がない場合のみ、その列全てのブロックが消せます。このとき消した星ブロックの数だけスコアが増えます。また、このとき爆弾ブロックを消していれば爆弾ブロックのある行すべてのブロックも一気に消すことができ、その消された分の星ブロックもスコアに入ります。<br>
&emsp;そして、**仲間ブロック**の**グーイ**は他のどの**仲間ブロック**とも組み合わせて消すことができます。<br>
&emsp;また、コンボの数に応じて上からきらきらと呼ばれるものが降ってきます。これは着地時に一瞬星ブロックに変化し、それを利用しコンボをさらに増やすこともできます。そしてきらきらは生成された数分スコアが増えます。<br>

##### 基本操作
- 矢印キー

操作中のブロックを左右に動かしたり落下速度を速めることができます。

- V, Bキー

操作中のブロックをVキーなら左に90°、Bキーなら右に90°回転させます。

- スペースキー

ホールドに保存されているブロックがなければ、現在操作中のブロックをホールドに保存し次のブロックの操作に代わり、既にあった場合は保存中のブロックと操作中のブロックを交換します。

- escキー

&emsp;画面がゲーム画面からポーズ画面に切り替わります。ポーズ画面ではゲームを再開するかアプリを終了するか選択できます。<br>
&emsp;また、ポーズ画面はゲーム画面左上のポーズボタンを左クリックしても遷移可能です。

##### 表示データ
ゲーム画面右の枠内に表示されるデータです。<br>
内容は上から順に
- Next: 次に操作するブロック
- Difficulty: 現在選択中の難易度(現verでは「たいへん」しか遊べません)
- Hold: 追加されていればホールドに保存中のブロックを表示します
- デデデまたはマスターハンド: 4ターン周期でデデデがマスターハンドの表示に代わります。マスターハンドの表示に代わったら次のターンで盤面がせりあがるので注意してください。
- Score: 現在のスコア
- Combo: 現在のコンボ数: 2コンボ以上つながる際に数値が表示されます。
<br><br>

#### エンディング画面

&emsp;この画面では今回のゲーム得たスコアと過去のスコアランキングを表示します。
&emsp;そしてこの画面でゲームをまた始めるか、アプリを終了するか選択することが可能です。

### アプリ動作システム要件

|||
|:----|:---|
|OS|Windows 7 SP1 (64-bit) / Windows 8.1 (64-bit) / Windows 10 (64-bit) / Windows 11|
|CPU|IntelまたはAMD製のCPU|
|映像出力|モニタなど、何らかの映像出力装置があること|

<br><br>

### その他情報

#### ライセンス情報

Siv3Dのライセンス情報:
[LICENSE](https://github.com/Siv3D/OpenSiv3D/blob/main/LICENSE)<br><br>

#### example内の画像ファイルの引用元リンク

全て[任天堂公式サイト](https://www.nintendo.co.jp/index.html)または[Nintendo公式チャンネル](https://www.youtube.com/@NintendoJP/featured)、[星のカービィポータル](https://www.kirby.jp/)、または[カービィカフェ公式サイト](https://kirbycafe.jp/)、任天堂取扱説明書から引用
以下詳細

- blockフォルダ
Star.png, Rick.png, Coo.png, Kine.png, Bomb.png, Hard.blockは[任天堂](https://www.nintendo.co.jp/wii/vc/vc_kak/vc_kak_03.html)から全て引用<br>
同フォルダ内のGooey.pngは[Nintendo公式チャンネル](https://www.youtube.com/watch?v=CXEfJ2qdVW8&t=9s)から引用<br>

- backgroundフォルダ
BackGround.pngは[星のカービィポータル](https://www.kirby.jp/special/extra_aw/thelandscapesofdreamland/)から引用<br>
Board.pngは[Nintendo公式チャンネル](https://www.youtube.com/watch?v=5vs-a0eLsH0)から引用<br>
difficulty.pngは[Nintendo公式チャンネル](https://youtu.be/5vs-a0eLsH0?si=NivlCrIddIdgLTTv)から引用し色の編集を行った。<br>
Ending.pngは[星のカービィポータル](https://www.kirby.jp/special/extra_aw/pupupufriends/)より引用<br>
ScoreDDD.pngは[Nintendo公式チャンネル](https://youtu.be/90unpm-6IwE?si=UJBdyOEu4Xu7txBC) から引用しゲームに合わせ色付け編集を行った。<br>
MasterHand(Down/Up/MaxUp/MaxDown).pngの4つは全て[Nintendo公式チャンネル](https://youtu.be/90unpm-6IwE?si=UJBdyOEu4Xu7txBC)から引用しゲームに合わせ色付け編集を行った。<br>
Pause.pngは[星のカービィ夢の泉の物語取扱説明書](http://setsumei.html.xdomain.jp/famicom/hoshinokirby/hoshinokirby.html#5)より引用<br>
Title.pngは[星のカービィポータル](https://www.kirby.jp/special/photoframe/)と[星のカービィポータル](https://www.kirby.jp/special/extra_aw/thelandscapesofdreamland/)より引用し、編集を行った。<br>
Tutorial.pngは[カービィカフェ公式サイト](https://www.hallab.co.jp/products/kirbycafecd/)より引用し、編集も行った。<br>
