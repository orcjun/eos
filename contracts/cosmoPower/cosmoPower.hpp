/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>

class cosmoPower : public eosio::contract {
   private:
      struct account {
         account_name owner;
         uint64_t     balance;
         uint64_t primary_key()const { return owner; }
      };

   public:
      cosmoPower( account_name self );

      void transfer( account_name from, account_name to, uint64_t quantity );
      void issue( account_name to, uint64_t quantity );
      void burn( account_name to, uint64_t quantity );
      void defer_exchange( account_name power, account_name coin, uint64_t powerQuantity);
      auto getPowerAccount( account_name inputName );

   private:
      eosio::multi_index<N(accounts), account> _accounts;

      void add_balance( account_name payer, account_name to, uint64_t q );
      void sub_balance( account_name payer, account_name to, uint64_t q );
      void change_balance( account_name powerFrom, account_name coinTo, uint64_t q);
};


