/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>

class cosmoCoin : public eosio::contract {
   private:
      struct account {
         account_name owner;
         uint64_t     balance;
         uint64_t primary_key()const { return owner; }
      };

   public:
      cosmoCoin( account_name self );

      void transfer( account_name from, account_name to, uint64_t quantity );
      void issue( account_name to, uint64_t quantity );
      void burn( account_name to, uint64_t quantity );
      void defer_exchange( account_name coin, account_name power, uint64_t coinQuantity);
      auto getCoinAccount( account_name inputName );

   private:
      eosio::multi_index<N(accounts), account> _accounts;

      void add_balance( account_name payer, account_name to, uint64_t q );
      void sub_balance( account_name payer, account_name to, uint64_t q );
      void change_balance( account_name coinFrom, account_name powerTo, uint64_t q);
};
