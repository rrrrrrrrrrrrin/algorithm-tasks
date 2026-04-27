#ifndef AATREE_H
#define AATREE_H

class aatree {
private:
	int val_;
	int level_;
	aatree* l = nullptr;
	aatree* r = nullptr;

public:
	explicit aatree(int val, int level)
		: val_(val),
		level_(level),
		l{ nullptr },
		r{ nullptr } {
	}

	aatree(const aatree&) = delete;
	aatree& operator=(const aatree&) = delete;

	// Skew is a right rotation to replace a subtree
	// containing a left horizontal link with one
	// containing a right horizontal link instead
	aatree skew(aatree* t) {
		if (t == nullptr) {
			return;
		}
		else if (t->l == nullptr) {
			return t;
		}
		else if (t->l->level_ == t->level_) {
			return aatree(t->l, t->l->level_, t->l->l, aatree(t, t->level_, t->l->r, t.r));
		}
		else {
			return t;
		}
	}

	// Split is a left rotation and level increase to replace a subtree
	// containing two or more consecutive right horizontal links with one
	// containing two fewer consecutive right horizontal links
	aatree split(aatree* t) {
		if (t == nullptr) {
			return;
		}
		else if (t->r == nullptr || t->r->r == nullptr) {
			return t;
		}
		else if (t->level_ == t->r->r->level_) {
			return aatree(t->r, t->r->level_ + 1, aatree(t, t->level_, t->l, t->r->l), t->r.r);
		}
	}

	aatree insert(int x, aatree* t) {
		if (t == nullptr) {
			return aatree(x, 1, nullptr, nullptr);
		}
		else if (x < t->val_) {
			t->l = insert(x, t->l);
		}
		else if (x > t->val_) {
			t->r = insert(x, t->r);
		}
		// else if: x == t->val, then: ignore

		t = skew(t);
		t = split(t);

		return t;
	}

	aatree decreaseLevel(aatree* t) {
		auto new_level = min(t->l->level_, t->r->level_) + 1;
		if (new_level < t->level_) {
			t->level_ = new_level;
			if (new_level < t->r->level_) {
				t->r->level_ = new_level;
			}
		}
		return t;
	}

	static inline bool leaf(aatree* t) {
		return t->level_ == 1;
	}

	// Smallest elem in right subtree
	static inline aatree* sucessor(aatree* t) {
		t = t->r;
		while (t && t->l) {
			t = t->l;
		}
		return t;
	}

	// Largest elem in left subtree
	static inline aatree* predecessor(aatree* t) {
		t = t->l;
		while (t && t->r) {
			t = t->r;
		}
		return t;
	}

	aatree delete_x(int x, aatree* t) {
		if (t == nullptr) {
			return t;
		}
		else if (x > t->val_) {
			t->r = delete(x, t->r);
		}
		else if (x < t->val_) {
			t->l = delete(x, t->l);
		}
		else {
			if (leaf(t)) {
				return nullptr;
			}
			else if (t->l == nullptr) {
				l = successor(t);
				t->r = delete_x(l->val_, t->r);
				t->val_ = l->val_;
			}
			else {
				l = predecessor(t);
				t->l = delete_x(l->val_, t->l);
				t->val_ = l->val_;
			}
		}

		t = decreaseLevel(t);

		t = skew(t);
		t->r = skew(t->r);
		if (t.r != nullptr) {
			t->r->r = skew(t->r->r);
		}

		t = split(t);
		t.r = split(t.r);
		return t;
	}

	bool belongs_to_set(int x, aatree* t) const {
		while (t) {
			if (x < t->val_) {
				t = t->l;
			}
			else if (x > t->val_) {
				t = t->r;
			}
			else {
				return true;
			}
		}
		return false;
	}

	/*inline int rootLevel() const {
		return root ? root->level : 0;
	}*/
};

#endif