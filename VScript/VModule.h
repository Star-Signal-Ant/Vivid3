#pragma once
#include "VName.h"
#include "VClass.h"

class VEnum;

class VModule
{
public:
	VModule();
	void SetName(VName name);
	void AddClass(VClass* cls);
	void AddEnum(VEnum* e);
	VName GetName();
	VModule* Clone();
	VClass* FindClass(std::string name);
	std::vector<VClass*> GetClasses() {
		return m_Classes;
	}
	std::vector<VEnum*> GetEnums() {
		return m_Enums;
	}
private:

	VName m_Name;
	std::vector<VClass*> m_Classes;
	std::vector<VEnum*> m_Enums;
};

