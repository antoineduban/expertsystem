#pragma once

#include "StrongPointer.h"
#include "ErrorCode.h"
#include "Rule.h"

namespace ft
{
	class RulesManager : public CountableSPtr
	{
		public:

			RulesManager();
			~RulesManager();

			inline const std::vector<Rule>&	GetRules() const	{ return m_oRules; }

			void	Reset();
			bool	AddRule(const Rule& oRule);
			bool	CheckRules() const;
			void	DivideRules();
			void	PrintRules() const;

		private:
			
			std::vector<Rule>	m_oRules;

			void	DivideBidirectionnalRules();
	};
}
