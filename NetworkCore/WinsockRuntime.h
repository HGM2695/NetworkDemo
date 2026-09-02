#pragma once

namespace gm
{
	class WinsockRuntime
	{
	public:
		WinsockRuntime() = default;
		~WinsockRuntime();

		WinsockRuntime(const WinsockRuntime&) = delete;
		WinsockRuntime& operator=(const WinsockRuntime&) = delete;

		bool Initialize();
		void Shutdown();
		bool IsInitialized() const;

	private:
		bool _isInitialized = false;
	};
}