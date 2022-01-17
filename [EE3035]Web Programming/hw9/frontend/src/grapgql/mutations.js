import { gql } from '@apollo/client';
export const CREATE_CHATBOX_MUTATION = gql`
    mutation createChatBox(
        $name1: String!
        $name2: String!
    ){
        createChatBox(name1: $name1, name2: $name2){
            messages{
                sender{
                    name
                }
                body
            }
        }
    }
`
export const CREATE_MESSAGE_MUTATION = gql`
    mutation createMessage(
        $sender: String!
        $receiver: String!
        $body: String!
    ){
        createMessage(sender: $sender, receiver: $receiver, body: $body){
            sender{
                name
            }
            body
        }
    }
`

export const CREATE_USER_MUTATION = gql`
    mutation createUser(
        $name: String!
        $password: String!
    ){
        createUser(name: $name, password: $password){
            name
        } 
    }
`

export const LOGIN_MUTATION = gql`
    mutation login(
        $name: String!
        $password: String!
    ){
        login(name: $name, password: $password){
            message
            password
        }
    }
`

export const SIGNUP_MUTATION = gql`
    mutation signup(
        $name: String!
        $password: String!
    ){
        signup(name: $name, password: $password){
            message
            password
        }
    }
`