#pragma once

namespace K3D12 {
	class NonCopyable {
	private:

	protected:
		NonCopyable() {};
		~NonCopyable() {};
	public:
	private:
	protected:
		void operator =(const NonCopyable& src) {};
		NonCopyable(const NonCopyable& src) {};
		void operator =(NonCopyable&& src) {};
		NonCopyable(NonCopyable&& src) {};
	public:
	};
}