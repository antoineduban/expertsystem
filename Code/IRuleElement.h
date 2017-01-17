#pragma once

namespace ft
{
	//////////////////////////////
	// Interface				//
	//////////////////////////////

	// Element de base de la RuleStack
	// les classes Operator et Variable en h�ritent

	class IRuleElement
	{
	public:

		enum EType
		{
			E_NONE = -1,
			E_OPERATOR = 0,
			E_VARIABLE
		};

	public:

		IRuleElement();
		virtual ~IRuleElement() = 0;

		virtual EType	GetType() const = 0;
	};
}