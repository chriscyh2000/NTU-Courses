import { gql } from '@apollo/client';
export const CHATBOX_QUERY = gql`
    query chatbox(
        $name: String!
    ){
        chatbox(name: $name){
            id,
            name,
            messages{
                sender{
                    name
                }
                body
            }
        } 
    }
`
export const USER_QUERY = gql`
    query user(
        $name: String!
    ){
        user(name: $name){
            password
        }
    }
        
`