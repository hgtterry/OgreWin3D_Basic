#pragma once
class CL64_ResourceListener : public ResourceGroupListener
{
public:
	CL64_ResourceListener(void);
	~CL64_ResourceListener(void);

	void resourceCreated(const ResourcePtr& resource);

	void resourceGroupLoadEnded(const String& groupName);

	void resourcePrepareEnded(void);

	void resourceLoadEnded(void);

	void resourceRemove(const ResourcePtr& resource);

	bool flag_Monitor_Resource;

};

