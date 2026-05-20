#pragma once

#include <algorithm>
#include <cstdio>
#include <set>
#include "geometry.hh"

template <typename T>
class Finder {
public:
	Finder() {}

	void add(const T *t)
	{
		elements_.insert(t);
	}
	void update(const T *t);
	void remove(const T *t)
	{
		elements_.erase(t);
	}

	/**
	 * @brief Retorna el conjunt d'objectes amb rectangles total o
	 *        parcialment dins de `rect`.
	 *
	 * Si el nombre de rectangles del contenidor és `n`, el cost
	 * de l'algorisme ha de ser O(log n).
	 *
	 * @param rect El rectangle de cerca
	 *
	 * @returns Un conjunt de punters a objectes que tenen un rectangle
	 *          parcial o totalment dins de `rect`
	 */
	std::set<const T *> query(pro2::Rect rect) const
	{
		std::set<const T *> result;

		for (	
			auto it = std::lower_bound(elements_.begin(), elements_.end(), rect.left,
				[](const T* elem, int left_value) {
					return elem->get_rect().right < left_value;
				});
			it != elements_.end() && (*it)->get_rect().left <= rect.right;
			it++
		)
			result.insert(*it);
		return result;
	}

private:
	struct Comparator {
		bool operator()(const T *A, const T *B) const
		{
			return A->get_rect().left < B->get_rect().left;
		}
	};

	std::set<const T *, Comparator> elements_;
};

