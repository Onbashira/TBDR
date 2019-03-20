#pragma once
#include <memory>
#include <map>
#include <string>


namespace K3D12 {
	//シンプルなリソースキャリアーのクラス
	template <class T>
	class ManagerComponentBase
	{
	private:
	protected:
		std::map<std::string, std::shared_ptr<T>> _resourceMap;
	public:
	private:
	public:
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, const T& resource, bool forceOverride = false);
		//要素の取得
		std::weak_ptr<T>	 Get(std::string name);
		//要素の削除
		void Erase(std::string name);
		//マップの明示的クリア
		void DiscardMap();
		//リソースがロード済みか否か
		bool IsLooded(std::string name);

		ManagerComponentBase() {};
		virtual ~ManagerComponentBase() { DiscardMap(); };
	};

	template <class T>
	class NonRaiiManagerComponentBase
	{
	private:
	protected:
		std::map<std::string, T> _resourceMap;
	public:
	private:
	public:
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, T& resource, bool forceOverride = false);
		//要素の取得
		T*	 Get(std::string name);
		//要素の削除
		void Erase(std::string name);
		//マップの明示的クリア
		void DiscardMap();
		//リソースがロード済みか否か
		bool IsLooded(std::string name);

		NonRaiiManagerComponentBase() {};
		virtual ~NonRaiiManagerComponentBase() { DiscardMap(); };
	};

}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> K3D12::ManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();
}

template<class T>
inline bool K3D12::ManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}


//NoRaii

template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::Set(std::string name, T& resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::move(resource);
	}
	else if (forceOverride) {
		_resourceMap[name] = std::move(resource);
	}
}

template<class T>
inline T * K3D12::NonRaiiManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return &_resourceMap[name];
	}
	return nullptr;
}

template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();

}

template<class T>
inline bool K3D12::NonRaiiManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}
