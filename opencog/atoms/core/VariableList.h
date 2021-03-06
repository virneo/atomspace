/*
 * opencog/atoms/core/VariableList.h
 *
 * Copyright (C) 2015 Linas Vepstas
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_VARIABLE_LIST_H
#define _OPENCOG_VARIABLE_LIST_H

#include <map>

#include <opencog/atomspace/Handle.h>
#include <opencog/atomspace/Link.h>

namespace opencog
{
/** \addtogroup grp_atomspace
 *  @{
 */

typedef std::map<Handle, const std::set<Type> > VariableTypeMap;

/// The Variables struct defines a list of variables in a way that
/// makes it easier and faster to work with in C++.  It implements 
/// the data that is shared between the VariableList link atom
/// and the pattern matcher.
///
struct Variables
{
	/// Unbundled variables and types for them.
	/// _typemap is the (possibly empty) list of restrictions on
	/// the variable types. The _varset contains exactly the same atoms
	/// as the _varseq; it is used for fast lookup; (i.e. is some
	/// some variable a part of this set?) whereas the _varseq list
	/// preserves the original order of the variables.  Yes, the fast
	/// lookup really is needed!
	///
	/// The _index is a reversed index into _varseq: given a variable,
	/// it returns the ordinal of that variable in the _varseq. It is
	/// used to implement the variable substitution (aka beta-reducation
	/// aka "PutLink") method.
	HandleSeq varseq;
	std::set<Handle> varset;
	VariableTypeMap typemap;
	std::map<Handle, unsigned int> index;

	// Return true if we are holding a single variable, and the handle
	// given as the argument satisfies the type restrictions (if any).
	// Else return false.
	bool is_type(const Handle& h) const;

	// Return true if the sequence is of the same length as the variable
	// declarations we are holding, and if they satisfy all of the type
	// restrictions (if any).
	bool is_type(const HandleSeq& hseq) const;

	// Given the tree `tree` containing variables in it, create and
	// return a new tree with the indicated values `vals` substituted
	// for the variables. The vals must pass the typecheck, else an
	// exception is thrown. An exception is thrown if the vals are not
	// of the types specified in this class.
	Handle substitute(const Handle& tree, const HandleSeq& vals) const;

	// Like the above, except no type-checking is done.
	Handle substitute_nocheck(const Handle&, const HandleSeq&) const;
};


/// The VariableList class records it's outgoing set in various ways
/// that make it easier and faster to work with in C++.  It implements
/// a substitute method that will replace all variables in a tree by
/// the corresponding atoms that it is given. See the .cc file for
/// more info.
///
/// The constructors make sure that the contents of the variable list
/// are syntactically correct; i.e that it actually contains variables.
/// Otherwise, it throws an error on bad syntax.  Thus, bad
/// VariableLists cannot be inserted into the atomspace.
class VariableList : public Link
{
protected:
	/// Unbundled variables and types for them.
	Variables _varlist;

	// See VariableList.cc for comments
	void get_vartype(const Handle&);

	// Validate the variable decls
	void validate_vardecl(const Handle&);
	void validate_vardecl(const HandleSeq&);

	VariableList(Type, const HandleSeq&,
	           TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	           AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	void build_index(void);
public:
	VariableList(const HandleSeq& vardecls,
	           TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	           AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	VariableList(Link&);

	// Return the list of variables we are holding.
	const Variables& get_variables(void) const { return _varlist; }

	// Return true if we are holding a single variable, and the handle
	// given as the argument satisfies the type restrictions (if any).
	// Else return false.
	bool is_type(const Handle& h) const { return _varlist.is_type(h); }

	// Return true if the sequence is of the same length as the variable
	// declarations we are holding, and if they satisfy all of the type
	// restrictions (if any).
	bool is_type(const HandleSeq& hseq) const { return _varlist.is_type(hseq); }

	// Given the tree `tree` containing variables in it, create and
	// return a new tree with the indicated values `vals` substituted
	// for the variables. The vals must pass the typecheck, else an
	// exception is thrown.
	Handle substitute(const Handle& tree, const HandleSeq& vals) const
		{ return _varlist.substitute(tree, vals); }
};

typedef std::shared_ptr<VariableList> VariableListPtr;
static inline VariableListPtr VariableListCast(const Handle& h)
	{ AtomPtr a(h); return std::dynamic_pointer_cast<VariableList>(a); }
static inline VariableListPtr VariableListCast(AtomPtr a)
	{ return std::dynamic_pointer_cast<VariableList>(a); }

// XXX temporary hack ...
#define createVariableList std::make_shared<VariableList>

/** @}*/
}

#endif // _OPENCOG_VARIABLE_LIST_H
