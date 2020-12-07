
#pragma once

#include <string>
#include <vector>
#include "ViewTree.h"

class CScopeToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CScopeView : public CDockablePane
{
public:
	CScopeView() noexcept;
	virtual ~CScopeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CScopeToolBar m_wndToolBar;
	CViewTree m_wndScopeView;
	CImageList m_ScopeViewImages;
	UINT m_nCurrSort;

	void FillScopeView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnScopeAddMemberFunction();
	afx_msg void OnScopeAddMemberVariable();
	afx_msg void OnScopeDefinition();
	afx_msg void OnScopeProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()


	bool CollectFiles(const std::wstring& path, const WIN32_FIND_DATA& data, bool Children);  // callback function to build tree
private:
	struct FileData
	{
		std::wstring path;
		bool directory;
		FILETIME ftCreationTime;
		FILETIME ftLastWriteTime;
	};
	std::vector<FileData> files_{};
	HTREEITEM hRoot{};
	HTREEITEM hScope{};

};

