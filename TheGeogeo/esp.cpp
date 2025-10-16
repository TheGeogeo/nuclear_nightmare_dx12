#include "includes.h"

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#ifndef IM_PI
#define IM_PI 3.14159265358979323846f
#endif

namespace esp
{
	void HealthBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float health, float maxHealth, FLOAT aLineWidth)
	{
		if (TopBox == SDK::FVector2D() || BottomBox == SDK::FVector2D()) return;
		if (health > 0 && maxHealth > 0)
		{
			float procentageHp = health / maxHealth * 100;
			overlay::HealthBar({ (float)TopBox.X - 4, (float)TopBox.Y }, { (float)BottomBox.X - 4, (float)BottomBox.Y }, aLineWidth, procentageHp);
		}
	}

	void ArmorBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float armor, float maxArmor, FLOAT aLineWidth)
	{
		if (TopBox == SDK::FVector2D() || BottomBox == SDK::FVector2D()) return;
		if (armor > 0 && maxArmor > 0)
		{
			float procentageArmor = armor / maxArmor * 100;
			overlay::ShieldBar({ (float)TopBox.X - 8, (float)TopBox.Y }, { (float)BottomBox.X - 8, (float)BottomBox.Y }, aLineWidth, procentageArmor);
		}
	}
}

namespace overlay
{
	ImFont* Verdana, * DefaultFont;

	typedef struct
	{
		DWORD R;
		DWORD G;
		DWORD B;
		DWORD A;
	}RGBA;

	std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return retStr;
	}

	auto DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddLine(aPoint1, aPoint2, aColor, aLineWidth);
	}

	auto DrawBox(float x, float y, float w, float h, ImColor color) -> VOID
	{
		DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top
		DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
		DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
		DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.3f);   // bottom
	}
	auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
	}

	auto RectNotFilled(ImVec2 t1, ImVec2 t2, ImVec2 b1, ImVec2 b2, ImColor color, FLOAT aLineWith) -> VOID
	{
		DrawLine(t1, t2, color, aLineWith);
		DrawLine(t2, b1, color, aLineWith);
		DrawLine(b1, b2, color, aLineWith);
		DrawLine(b2, t1, color, aLineWith);
	}

	void HealthBar(ImVec2 t1, ImVec2 b1, float aLineWith, int health)
	{
		auto vList = ImGui::GetBackgroundDrawList();
		if (!vList) return;  // Guard against null pointers.

		if (health > 100) health = 100;
		float percent = (float)health / 100;
		percent = (b1.y - t1.y) * percent;

		ImColor barColor = ImColor(
			min(510 * (100 - health) / 100, 255),
			min(510 * health / 100, 255),
			25,
			255
		);

		vList->AddLine(b1, { t1.x, b1.y - percent }, barColor, aLineWith);
	}

	void ShieldBar(ImVec2 t1, ImVec2 b1, float aLineWith, int shield)
	{
		auto vList = ImGui::GetBackgroundDrawList();
		if (!vList) return;  // Guard against null pointers.

		if (shield > 100) shield = 100;
		float percent = (float)shield / 100;
		percent = (b1.y - t1.y) * percent;

		vList->AddLine(b1, { t1.x, b1.y - percent }, ImColor{ 10,10,255 }, aLineWith);
	}

	auto DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID
	{
		ImGui::PushFont(Verdana);
		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int index = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{//
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (index + 1);
			index++;
		}
		ImGui::PopFont();
	}

	auto DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();

		float lineW = (W / 3);
		float lineH = (H / 3);
		//black outlines
		auto col = ImGui::GetColorU32(color);

		//corners
		vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
		vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
		vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

		vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
		vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
		vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);
	}

	auto DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(aPoint, aR, aColor, 120, aLineWidth);
	}
	auto DrawCircleFilled(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetForegroundDrawList();
		vList->AddCircleFilled(aPoint, aR, aColor, 120);
	}
	auto DrawCircle(float x, float y, float radius, ImVec4 color, int segments) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
	}
	auto DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddText(aPos, aColor, aString.data());
	}
	auto TabButton(const char* label, int* index, int val, bool sameline, ImVec2 size) -> VOID
	{
		if (*index == val)
		{
			if (ImGui::Button(label, size))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
		else
		{
			if (ImGui::Button(label, size))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
	}

	void DrawCircleFilledCursor()
	{
		auto vList = ImGui::GetForegroundDrawList();

		POINT p;
		GetCursorPos(&p);

		vList->AddCircleFilled(ImVec2{ (float)p.x, (float)p.y }, 4.f, ImColor{ 255,20,20 });
	}

	void DefaultImGuiItemWidth()
	{
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
	}

	void SetImGuiItemWidth(float val)
	{
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * val);
	}

	void Draw3dBox(SDK::AActor* actor, ImColor color)
	{
		SDK::FVector origin, extent;
		actor->GetActorBounds(true, &origin, &extent, true);
		SDK::FRotator rotation = actor->K2_GetActorRotation();
		float yaw = ue->MathLib->DegreesToRadians((int)(rotation.Yaw + 450.0f) % 360);

		extent.Z = extent.Z;
		SDK::FVector t1 = origin, t2 = origin, t3 = origin, t4 = origin, b1 = origin, b2 = origin, b3 = origin, b4 = origin;
		t1.X -= 35.f;
		t1.Y -= 35.f;
		t1.Z -= extent.Z;
		t2.X += 35.f;
		t2.Y -= 35.f;
		t2.Z -= extent.Z;
		t3.X += 35.f;
		t3.Y += 35.f;
		t3.Z -= extent.Z;
		t4.X -= 35.f;
		t4.Y += 35.f;
		t4.Z -= extent.Z;

		t1 = ue->RotateCorner(origin, t1, yaw);
		t2 = ue->RotateCorner(origin, t2, yaw);
		t3 = ue->RotateCorner(origin, t3, yaw);
		t4 = ue->RotateCorner(origin, t4, yaw);

		SDK::FVector2D ts1, ts2, ts3, ts4;
		if (!ue->PlayerController->ProjectWorldLocationToScreen(t1, &ts1, false) || !ue->PlayerController->ProjectWorldLocationToScreen(t2, &ts2, false) || !ue->PlayerController->ProjectWorldLocationToScreen(t3, &ts3, false) || !ue->PlayerController->ProjectWorldLocationToScreen(t4, &ts4, false))
		{
			return;
		}

		b1.X -= 35.f;
		b1.Y -= 35.f;
		b1.Z += extent.Z;
		b2.X += 35.f;
		b2.Y -= 35.f;
		b2.Z += extent.Z;
		b3.X += 35.f;
		b3.Y += 35.f;
		b3.Z += extent.Z;
		b4.X -= 35.f;
		b4.Y += 35.f;
		b4.Z += extent.Z;

		b1 = ue->RotateCorner(origin, b1, yaw);
		b2 = ue->RotateCorner(origin, b2, yaw);
		b3 = ue->RotateCorner(origin, b3, yaw);
		b4 = ue->RotateCorner(origin, b4, yaw);

		SDK::FVector2D bs1, bs2, bs3, bs4;
		if (!ue->PlayerController->ProjectWorldLocationToScreen(b1, &bs1, false)
			|| !ue->PlayerController->ProjectWorldLocationToScreen(b2, &bs2, false)
			|| !ue->PlayerController->ProjectWorldLocationToScreen(b3, &bs3, false)
			|| !ue->PlayerController->ProjectWorldLocationToScreen(b4, &bs4, false))
		{
			return;
		}

		overlay::DrawLine({ (float)ts1.X,(float)ts1.Y }, { (float)ts2.X, (float)ts2.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts2.X,(float)ts2.Y }, { (float)ts3.X, (float)ts3.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts3.X,(float)ts3.Y }, { (float)ts4.X, (float)ts4.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts4.X,(float)ts4.Y }, { (float)ts1.X, (float)ts1.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs1.X,(float)bs1.Y }, { (float)bs2.X, (float)bs2.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs2.X,(float)bs2.Y }, { (float)bs3.X, (float)bs3.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs3.X,(float)bs3.Y }, { (float)bs4.X, (float)bs4.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs4.X,(float)bs4.Y }, { (float)bs1.X, (float)bs1.Y }, color, 1.f);

		overlay::DrawLine({ (float)ts1.X,(float)ts1.Y }, { (float)bs1.X, (float)bs1.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts2.X,(float)ts2.Y }, { (float)bs2.X, (float)bs2.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts3.X,(float)ts3.Y }, { (float)bs3.X, (float)bs3.Y }, color, 1.f);
		overlay::DrawLine({ (float)ts4.X,(float)ts4.Y }, { (float)bs4.X, (float)bs4.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs1.X,(float)bs1.Y }, { (float)ts1.X, (float)ts1.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs2.X,(float)bs2.Y }, { (float)ts2.X, (float)ts2.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs3.X,(float)bs3.Y }, { (float)ts3.X, (float)ts3.Y }, color, 1.f);
		overlay::DrawLine({ (float)bs4.X,(float)bs4.Y }, { (float)ts4.X, (float)ts4.Y }, color, 1.f);
	}

	auto DrawSquare(const ImVec2& aCenter, const FLOAT aSize, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		ImDrawList* vList = ImGui::GetBackgroundDrawList();
		const float half = aSize * 0.5f;
		const ImVec2 pMin(aCenter.x - half, aCenter.y - half);
		const ImVec2 pMax(aCenter.x + half, aCenter.y + half);
		vList->AddRect(pMin, pMax, aColor, 0.0f, 0, aLineWidth);
	}

	auto DrawRect(const ImVec2& aCenter, const FLOAT aWidth, const FLOAT aHeight, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		ImDrawList* vList = ImGui::GetBackgroundDrawList();
		const ImVec2 half(aWidth * 0.5f, aHeight * 0.5f);
		const ImVec2 pMin(aCenter.x - half.x, aCenter.y - half.y);
		const ImVec2 pMax(aCenter.x + half.x, aCenter.y + half.y);
		vList->AddRect(pMin, pMax, aColor, 0.0f, 0, aLineWidth);
	}

	auto DrawTriangle(const ImVec2& aCenter, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth, const FLOAT aRotationDeg) -> VOID
	{
		ImDrawList* vList = ImGui::GetBackgroundDrawList();

		const float base = aRotationDeg * (IM_PI / 180.0f);
		const float a1 = base;
		const float a2 = base + 2.0f * IM_PI / 3.0f;
		const float a3 = base + 4.0f * IM_PI / 3.0f;

		const ImVec2 p1(aCenter.x + std::cos(a1) * aR, aCenter.y + std::sin(a1) * aR);
		const ImVec2 p2(aCenter.x + std::cos(a2) * aR, aCenter.y + std::sin(a2) * aR);
		const ImVec2 p3(aCenter.x + std::cos(a3) * aR, aCenter.y + std::sin(a3) * aR);

		vList->AddTriangle(p1, p2, p3, aColor, aLineWidth);
	}

	auto DrawHexagon(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(aPoint, aR, aColor, 6, aLineWidth);
	}
}