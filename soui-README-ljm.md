## 代码阅读建议

基础资料：
布局及其属性：https://www.cnblogs.com/setoutsoft/p/3925952.html
资源说明文档：https://www.cnblogs.com/setoutsoft/p/3923610.html





### 入门学习
1.学习换肤弹窗
初始化代码：
		CSetSkinWnd *pSetSkinWnd = new CSetSkinWnd();
		pSetSkinWnd->Create(NULL);
		pSetSkinWnd->CenterWindow(GetDesktopWindow());
		pSetSkinWnd->ShowWindow(SW_SHOWDEFAULT);
		m_hSetSkinWnd = pSetSkinWnd->m_hWnd;


换肤窗口定义使用的资源名称;CSetSkinWnd::CSetSkinWnd() :SHostWnd(_T("LAYOUT:dlg_set_skin"))
对应的xml文件是: 		<file name="dlg_set_skin" path="xml\dlg_skinset.xml" /> ，此隐射关系在uires.idx文件中定义


换肤点击事件绑定：EVENT_ID_RANGE_HANDLER(10, 27, EVT_CMD, OnSetSkin)
换肤完成之后需要发送消息通知主窗口，发送系统级别的广播消息，主窗口和换肤窗口注册了消息监听
：	return ::BroadcastSystemMessage(BSF_POSTMESSAGE, &dwRecipients, g_dwSkinChangeMessage, wParam, lParam);

回调函数中会刷新页面：FindChildByID(9527)->Invalidate();
9527是当前主窗口id在文件dlg_main.xml中定义 （window pos="1,1,-1,-1" id="9527" ）

