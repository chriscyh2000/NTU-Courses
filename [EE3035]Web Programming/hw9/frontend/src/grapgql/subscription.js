import { gql } from '@apollo/client';
export const CHATBOX_SUBSCRIPTION = gql`
    subscription chatBox(
        $name: String!
    ){
        chatBox(name: $name){
            data {
                body
                sender{
                    name
                }
            }
        }
    }
`
