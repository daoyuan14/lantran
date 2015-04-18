## Introduction ##

LanTran is a Full Featured File Transfer Tool For Local Area Networks(LAN).

## News ##

  * 2008-12-05: Finished developing LanTran v0.5.0;
  * 2010-08-25: Release LanTran v0.5.0 to Open Source;

## Features ##

Especially concerning the security of communication when users transfer their files.

  1. **自动关联杀毒：**<br>这个功能是我们最有创新的一个地方。灵感来源：我们知道迅雷可以通过和瑞星等企业合作对下载完成的文件进行自动查杀，这样就避免了用户手动进行对下载完成的文件再进行查杀了。虽然我们不能像迅雷一样获得那些杀毒软件提供的接口，但我们通过编程方法也实现了对我们传输过去的文件进行自动查杀。这样就满足了那些对安全系数要求很高的用户。<br>
<ol><li><b>安全的加密传输：</b><br>通过分析企业内部用户传输数据的需求，我们增加了这个模块来满足这些需求。总体上使用RSA加密算法，很难被破解。无论是口令传输和不含口令的传输，我们都采用了多报数据加密的方式，即奇数的数据包采用一种加密方法，偶数的数据包采用另一种加密方法。这样极大地增加了被破解的可能。而采用图片作为密钥的加密方法则更加安全，因为只有那些拥有同一张图片的用户才可以解密成功，安全系数更是达到了很高的境界。<br>
</li><li><b>取消了客户端服务器的概念，动态显示在线用户：</b><br>我们都知道，QQ、MSN都是有服务器的，但是这种方式并不适合于局域网用户。因此，我们在本软件中取消了客户端和服务器的概念：即每个用户都是客户端，亦是服务器。同时，为了用户的使用的方便性，我们自定义了协议规则、构造特定数据包，相比局域网广播方式能更有效率地动态显示所有局域网在线用户，避免了用户输入IP地址的麻烦，在可以体现出来。<br>
</li><li><b>快速且高效的文件传输：</b><br>通过优化I/O算法，使传输速度在有线连接的情况下能达到6~7M每秒，在无线连接的情况下能达到1M每秒；CPU占用率平均维持在10以内；基本实现了较高效较快速的文件传输。支持将文件直接拖入进行传输，并以图标的形式显示在框中，给用户带来爽心悦目的感觉。<br>
</li><li><b>附加功能：</b><br>在线聊天和语音聊天是我们基于在上述创新点的基础上作为完善我们这个软件而添加的功能，主要为了满足不同用户的需求。具有界面美观，使用方便，功能齐全等特点。</li></ol>

<b>Please see <a href='http://code.google.com/p/lantran/source/browse/#svn/trunk/doc'>these full docs</a>!</b> They are docs for the competition. Recommend it!<br>
<br>
<h2>Requirements</h2>

<ul><li>Only Windows now: XP SP2 & SP3, Vista Home Basic & Premium;</li></ul>

<h2>Screenshot</h2>

<img src='http://lantran.googlecode.com/files/1.jpg' />
<br>Figure: There are 4 users in a local area network connecting by a router