
#include "Application.h"

#include "Core.h"
#include "File.h"
#include "Lexer.h"
#include "VariablesManager.h"
#include "RulesManager.h"
#include "InferenceEngine.h"
#include "LogicOperator.h"
#include "Proposition.h"
#include "Parser.h"

#include "Output.h" // tmp

#include <stack>
#include <fstream>
#include <sstream>

namespace ft
{
	Application::Application()
		: m_xVariablesManager(nullptr)
		, m_xRulesManager(nullptr)
		, m_xInferenceEngine(nullptr)
	{
	}

	Application::~Application()
	{
	}

	EErrorCode	Application::Init(const int32 ac, const char* const* av)
	{
		m_xVariablesManager	= new VariablesManager();
		m_xRulesManager		= new RulesManager();
		m_xInferenceEngine	= new InferenceEngine();

		FT_TEST_OK(ReadInputFiles(ac, av));


		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		m_xVariablesManager.Release();
		m_xRulesManager.Release();
		m_xInferenceEngine.Release();

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// Vérifie que c'est initialisé
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		EvaluatePendingQueries();

		FT_TEST_OK(AskUserInput());

		return FT_OK;
	}

	EErrorCode	Application::ReadInputFiles(const int32 ac, const char* const* av)
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		std::ifstream		oIFStream;
		std::stringstream	oSStream;
		std::string			sLine;
		const char*			csFilePath;

		//for (int32 i = 1; i < ac; ++i)
		//{
		//	csFilePath = av[i];
			csFilePath = "./Assets/Regles_Simples_03.txt"; (void)ac; (void)av;
			oIFStream.open(csFilePath);
			if (oIFStream.rdstate() & std::ifstream::failbit)
			{
				FT_CERR << "Echec dans l'ouverture du fichier " << csFilePath << std::endl;
				return FT_FAIL;
			}
			oSStream << oIFStream.rdbuf();
			oIFStream.close();

			while (std::getline(oSStream, sLine))
			{
				if (!ProcessInputLine(sLine))
					break;
			}
		//}

		return FT_OK;
	}

	EErrorCode	Application::AskUserInput()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		std::string	sLine;

		while (m_ePendingCommand != E_QUIT)
		{
			std::cout << "AKINATOR T'ECOUTE MON ENFANT:" << std::endl;
			std::getline(std::cin, sLine);
			if (ProcessInputLine(sLine) != FT_OK)
			{
				FT_NOT_IMPLEMENTED("Erreur entree utilisateur");
				continue;
			}

			// debug
			{
				FT_COUT << "###################################" << std::endl;
				m_xVariablesManager->DebugPrint();
				m_xRulesManager->PrintRules();
				FT_COUT << "REQUETES: ";
				for (AtomIdSet::const_iterator itQuery = m_oPendingQueries.begin(), itEnd = m_oPendingQueries.end(); itQuery != itEnd; ++itQuery)
					FT_COUT << *itQuery;
				FT_COUT << std::endl;
				FT_COUT << "###################################" << std::endl;
			}
			//

			EvaluatePendingQueries();
		}

		return FT_OK;
	}

	EErrorCode	Application::ProcessInputLine(const std::string& sLine)
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		Lexer::OutData	oLexingData;
		Parser			oParser;
		Parser::OutData	oParsingData;

		FT_TEST_OK(Lexer::ReadLine(&oLexingData, sLine));
		FT_TEST_OK(oParser.ReadTokens(&oParsingData, oLexingData.oTokens));

		switch (oParsingData.eDataType)
		{
		case Parser::OutData::E_RULE:
			{
				if (!m_xRulesManager->AddRules(oParsingData.oRules))
				{
					FT_NOT_IMPLEMENTED("Cas d'erreur d'ajout de regles");
				}
				m_xVariablesManager->DeclareVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				break;
			}

		case Parser::OutData::E_FACTS:
			{
				m_xVariablesManager->SetVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end(), true);
				break;
			}

		case Parser::OutData::E_QUERIES:
			{
				m_xVariablesManager->DeclareVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				m_oPendingQueries.insert(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				break;
			}

		case Parser::OutData::E_COMMAND:
			{
				switch (oParsingData.eCommandType)
				{
				case Token::E_CMD_QUIT:
					{
						m_ePendingCommand = E_QUIT;
						break;
					}

				default:
					{
						FT_NOT_IMPLEMENTED("Commande inconnue")
						break;
					}
				}
				break;
			}

		default:
			break;
		}

		return FT_OK;
	}

	EErrorCode	Application::EvaluatePendingQueries()
	{
		bool	bEvaluation;
		m_pRulesManager->CheckRules();
		m_pRulesManager->DivideRules();

		if (m_oPendingQueries.size() > 0)
		{
			for (AtomIdSet::const_iterator itQuery = m_oPendingQueries.begin(), itEnd = m_oPendingQueries.end(); itQuery != itEnd; ++itQuery)
			{
				bEvaluation = m_xInferenceEngine->ProcessQuery(*m_xVariablesManager, m_xRulesManager->GetRules(), *itQuery);
				FT_COUT << "Evaluation de " << *itQuery << " : " << bEvaluation << std::endl;
			}
			m_oPendingQueries.clear();
		}

		return FT_OK;
	}
}

