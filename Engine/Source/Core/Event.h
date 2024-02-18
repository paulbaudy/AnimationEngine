#pragma once

#define DEFINE_EVENT() public:virtual void accept(IEventVisitor& v) const override { v.OnEvent(*this); }\

namespace AN
{
	class FEvent
	{
	public:
		virtual void accept(class IEventVisitor& v) const = 0;
	};

	class IEventVisitor
	{
	public:
		void OnEvent(const FEvent& InEvent)
		{
			InEvent.accept(*this);
		}
		virtual void OnEvent(const class FWindowCloseEvent& InEvent) {}
		virtual void OnEvent(const class FWindowResizeEvent& InEvent) {}
	};

	class FWindowCloseEvent : public FEvent
	{
		DEFINE_EVENT()
	};
	class FWindowResizeEvent : public FEvent
	{
		DEFINE_EVENT()
	};


}
