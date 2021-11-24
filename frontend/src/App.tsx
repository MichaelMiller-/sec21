import {BrowserRouter, Route, Switch} from 'react-router-dom';
import {Container} from "react-bootstrap";
import React from "react";
import ProjectList from "./ProjectList";
import {Project, RouterComponentProjectProps} from "./Project";

const App = () => {
    return (
        <Container fluid>
            <BrowserRouter>
                <Switch>
                    <Route exact path="/404">not found</Route>
                    <Route exact path="/"><ProjectList/></Route>
                    <Route
                        exact
                        path="/project/:projectId"
                        render={({match}: RouterComponentProjectProps) => (
                            <Project projectId={match.params.projectId}/>)}
                    />
                </Switch>
            </BrowserRouter>
        </Container>
    );
};

export default App;
