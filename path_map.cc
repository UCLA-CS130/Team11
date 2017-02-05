#include "path_map.h"

void PathMap::URIPathMap(const char* arg)
{

	NginxConfig config;
    NginxConfigParser parser;
    std::string key;
    std::string value;
    std::string temp;

    parser.Parse(arg, &config);

    
    for(int i = 0; i < config.statements_.size(); i++)
    {

    	temp = config.statements_[i]->tokens_[1];

    	if(isdigit(temp[0]))
    	{

    		std::string t2 = config.statements_[i]->tokens_[0];

    		if(t2 == "listen")
    		{
    			portNum = config.statements_[i]->tokens_[1];
    		}else 
    		{
    			std::cerr << "invalid" <<std::endl;

    		}

    	}else 
    	{
    		std::string check = config.statements_[i]->tokens_[1];
		    value = config.statements_[i]->child_block_->statements_[0]->tokens_[1];

		    std::string t3 = config.statements_[i]->tokens_[0];

		    if(t3 == "location")
		    {
		    	if (check[0] == '/')
		    	{
			    	key = check.substr(1);
					request[key] = value;

			    }else
			    {
			    	std::cerr << "invalid" <<std::endl;
			    }
		    }else
		    {
		    	std::cerr << "invalid" <<std::endl;

		    }
		    
    	}


	}

	std::cout << request["static2"] << std::endl;
	
	std::cout << portNum << std::endl;

	std::cout << request["static1"] << std::endl;

}