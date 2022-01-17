import { GraphQLServer, PubSub } from "graphql-yoga";

// resolvers
import Query from "./resolvers/Query.js";
import Mutation from "./resolvers/Mutation.js";
import DateResolver from "./resolvers/Date.js";
import StatusResolver from "./resolvers/Status.js";
import Subscription from "./resolvers/Subscription.js";
// db
import taskModel from "./models/task.js";

const pubSub = new PubSub();
const server = new GraphQLServer({
  typeDefs: "./src/schema.graphql",
  resolvers: {
    Query,
    Mutation,
    Subscription,
    Date: DateResolver,
    Status: StatusResolver,
  },
  context: {
    taskModel,
    pubSub,
  },
});

export default server;