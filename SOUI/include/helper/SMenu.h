﻿#pragma once

#include <core/sobjType.h>
#include <sobject/Sobject.hpp>
#include "core/SNativeWnd.h"
#include "res.mgr/Sskinpool.h"

namespace SOUI
{

#define CX_ICON    16        //支持的图标的宽度
#define CY_ICON    16        //支持的图标的高度

class SMenuAttr:public SObject, public TObjRefImpl<IObjRef>
{
    friend class SMenu;
	friend class SMenuODWnd;
    SOUI_CLASS_NAME(SMenuAttr, L"menuattr")
public:
    SMenuAttr();
	~SMenuAttr();
	

	void SetScale(int scale);

    SOUI_ATTRS_BEGIN()
        ATTR_SKIN(L"itemSkin",m_pItemSkin,FALSE)
        ATTR_SKIN(L"iconSkin",m_pIconSkin,FALSE)
        ATTR_SKIN(L"sepSkin",m_pSepSkin,FALSE)
        ATTR_SKIN(L"checkSkin",m_pCheckSkin,FALSE)
		ATTR_LAYOUTSIZE(L"itemHeight",m_nItemHei,FALSE)
		ATTR_LAYOUTSIZE(L"iconMargin",m_nIconMargin,FALSE)
		ATTR_LAYOUTSIZE(L"textMargin",m_nTextMargin,FALSE)
		ATTR_LAYOUTSIZE(L"maxWidth",m_nMaxWidth,FALSE)
		ATTR_LAYOUTSIZE2(L"iconSize",m_szIcon,FALSE)
        ATTR_FONT(L"font",m_dpiFont,FALSE)
        ATTR_COLOR(L"colorText",m_crTxtNormal,FALSE)
        ATTR_COLOR(L"colorTextSel",m_crTxtSel,FALSE)
        ATTR_COLOR(L"cororTextGray",m_crTxtGray,FALSE)
        ATTR_STRINGW(L"trCtx",m_strTrCtx,FALSE)
    SOUI_ATTRS_END()    

protected:
	int GetTextMargin();
	int GetIconMargin();
	CSize GetIconSize();
	int GetItemHeight();
	int GetMaxWidth();
	int GetScale() const{return m_scale;}

	SAutoRefPtr<IFont> GetFontPtr();
protected:
	virtual void OnInitFinished(pugi::xml_node xmlNode);

    SAutoRefPtr<ISkinObj>	m_pItemSkin;    //菜单项皮肤，包含2种状态：正常状态+选中状态
    SAutoRefPtr<ISkinObj>	m_pIconSkin;    //菜单图标
    SAutoRefPtr<ISkinObj>	m_pSepSkin;    //分割栏皮肤
    SAutoRefPtr<ISkinObj>	m_pCheckSkin;    //选中状态,包含两种状态:勾选+圈选
	SLayoutSize			m_nItemHei;    //菜单项高度
	SLayoutSize			m_nIconMargin;//图标边缘空间
	SLayoutSize			m_nTextMargin;//文本边缘空间
    COLORREF			m_crTxtNormal;//正常文本颜色
    COLORREF			m_crTxtSel;    //选中文本颜色
    COLORREF			m_crTxtGray;    //灰文本颜色
	SLayoutSize			m_szIcon[2];        //图标尺寸
    SDpiAwareFont		m_dpiFont;
    SStringW			m_strTrCtx;   //翻译上下文
	SLayoutSize			m_nMaxWidth;   //菜单项最大宽度
	int					m_scale;
};

struct SMenuItemData
{
	SMenuItemData();

	~SMenuItemData();
	int iIcon;
	SStringT strText;
	UINT vHotKey;
	ULONG_PTR dwUserData;
	HICON	 hIcon;
};

template <class T>
class SOwnerDraw
{
public:
    // Message map and handlers
    BEGIN_MSG_MAP_EX(SOwnerDraw< T >)
    MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
    MESSAGE_HANDLER(WM_COMPAREITEM, OnCompareItem)
    MESSAGE_HANDLER(WM_DELETEITEM, OnDeleteItem)
    MESSAGE_HANDLER(WM_MENUCHAR,OnMenuChar)
    ALT_MSG_MAP(1)
    MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(OCM_MEASUREITEM, OnMeasureItem)
    MESSAGE_HANDLER(OCM_COMPAREITEM, OnCompareItem)
    MESSAGE_HANDLER(OCM_DELETEITEM, OnDeleteItem)
    END_MSG_MAP()

    LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(TRUE);
        pT->DrawItem((LPDRAWITEMSTRUCT)lParam);
        bHandled = pT->IsMsgHandled();
        return (LRESULT)TRUE;
    }

    LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(TRUE);
        pT->MeasureItem((LPMEASUREITEMSTRUCT)lParam);
        bHandled = pT->IsMsgHandled();
        return (LRESULT)TRUE;
    }

    LRESULT OnCompareItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(TRUE);
        bHandled = pT->IsMsgHandled();
        return (LRESULT)pT->CompareItem((LPCOMPAREITEMSTRUCT)lParam);
    }

    LRESULT OnDeleteItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(TRUE);
        pT->DeleteItem((LPDELETEITEMSTRUCT)lParam);
        bHandled = pT->IsMsgHandled();
        return (LRESULT)TRUE;
    }

    LRESULT OnMenuChar(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(TRUE);
        
        UINT nChar = LOWORD(wParam);
        UINT nFlags = HIWORD(wParam);
        HMENU hMenu =(HMENU)lParam;
        LRESULT lRes = pT->OnMenuChar(nChar,nFlags,hMenu);
        
        bHandled = pT->IsMsgHandled();
        return lRes;
    }
    
    // Overrideables
    void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
    {
        // must be implemented
        SASSERT(FALSE);
    }

    void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
    {
        if(lpMeasureItemStruct->CtlType != ODT_MENU)
        {
            // return default height for a system font
            T* pT = static_cast<T*>(this);
            HWND hWnd = pT->GetDlgItem(lpMeasureItemStruct->CtlID);
            CClientDC dc(hWnd);
            TEXTMETRIC tm = { 0 };
            dc.GetTextMetrics(&tm);

            lpMeasureItemStruct->itemHeight = tm.tmHeight;
        }
        else
            lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU);
    }

    int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
    {
        // all items are equal
        return 0;
    }

    void DeleteItem(LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/)
    {
        // default - nothing
    }
};

class SMenuODWnd : public SNativeWnd
    ,public SOwnerDraw<SMenuODWnd>
{
    friend class SOwnerDraw<SMenuODWnd>;
public:
    SMenuODWnd(HWND hMenuOwner,SMenuAttr * pMenuAttr);

	~SMenuODWnd();
protected:
    void OnInitMenu(HMENU menu);
    void OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu);

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

    void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU menu);

    LRESULT OnMenuChar(UINT nChar,  UINT nFlags, HMENU hMenu);
        
    BEGIN_MSG_MAP_EX(SMenuODWnd)
		MSG_WM_INITMENU(OnInitMenu)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_MENUSELECT(OnMenuSelect)
		CHAIN_MSG_MAP(SOwnerDraw<SMenuODWnd>)
		REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    HWND m_hMenuOwner;
	SAutoRefPtr<SMenuAttr> m_attr;
};


class SOUI_EXP SMenu
{
public:
    SMenu();
	SMenu(const SMenu & src);
	SMenu(HMENU hMenu);
    ~SMenu(void);

	BOOL Attach(HMENU hMenu);

	HMENU Detach();

    BOOL LoadMenu(const SStringT & resId);

    BOOL LoadMenu(pugi::xml_node xmlMenu);

	void SetIconSkin(SAutoRefPtr<ISkinObj> icons);

	/**
     * SMenu::InsertMenu
     * @brief    插入一个菜单项,参数和Windows API类似。
     * @param    int iIcon -- 显示的标图在iconSkin中的索引
	 * @param    HICON hIcon -- 标图，默认为0，有值时，iIocn失效
	 * @return   BOOL - TRUE:成功 
     *
     * Describe  hIcon会在菜单退出时自动调用DestroyIcon.
     */
    BOOL InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR nIDNewItem,LPCTSTR strText, int iIcon=-1,HICON hIcon=0);

	BOOL AppendMenu(UINT uFlags,UINT_PTR uIDNewItem, LPCTSTR lpNewItem,int iIcon=-1,HICON hIcon=0);

	BOOL CheckMenuItem(UINT uIdCheckItem, UINT uCheck);

	BOOL DeleteMenu(UINT uPosition, UINT uFlags);

    UINT TrackPopupMenu( UINT uFlags, int x, int y, HWND hWnd, LPCRECT prcRect=NULL, int nScale = 100);

    void DestroyMenu();

	BOOL ModifyMenuString(UINT uPosition, UINT uFlags,LPCTSTR lpItemString);

	BOOL SetMenuUserData(UINT uPosition, UINT uFlags,ULONG_PTR ulUserData);

	ULONG_PTR GetMenuUserData(UINT uPosition, UINT uFlags);

    HMENU m_hMenu;

protected:
	void UpdateScale(int nScale);
    void BuildMenu(HMENU menuPopup,pugi::xml_node xmlNode);
    void InitMenuItemData(SMenuItemData * itemInfo, const SStringW & strText);
	void FreeMenuItemData(HMENU hMemu);

	SMenuAttr * GetMenuAttr(HMENU hMenu) const;
	void SetMenuAttr(HMENU hMenu,SMenuAttr *pMenuAttr) const;

	bool m_bAttached;
	SAutoRefPtr<ISkinObj> m_icons;
};

}//namespace SOUI