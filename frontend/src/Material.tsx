import { Button, Col, Form, Row, Table } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import { BackButton } from "./ui/BackButton";

export type MaterialProps = {
    projectId: string;
}

export function Material(props: MaterialProps) {
    const history = useHistory();

    const onBack = () => { history.push("/project/" + props.projectId); }

    return (
        <>
            <BackButton onClick={onBack} disabled={false} />
            <Row>
                <Form>
                    <Form.Group as={Row} controlId="formName">
                        <Form.Label column sm={2}>
                            Name</Form.Label>
                        <Col sm={10}>
                            <Form.Control placeholder="Enter name" />
                        </Col>
                    </Form.Group>
                    <Form.Row>

                        <Form.Group as={Col} controlId="formGridPassword">
                            <Form.Label>Password</Form.Label>
                            <Form.Control type="password" placeholder="Password" />
                        </Form.Group>
                    </Form.Row>

                    <Form.Group controlId="formGridAddress1">
                        <Form.Label>Address</Form.Label>
                        <Form.Control placeholder="1234 Main St" />
                    </Form.Group>

                    <Form.Group controlId="formGridAddress2">
                        <Form.Label>Address 2</Form.Label>
                        <Form.Control placeholder="Apartment, studio, or floor" />
                    </Form.Group>

                    <Form.Row>
                        <Form.Group as={Col} controlId="formGridCity">
                            <Form.Label>City</Form.Label>
                            <Form.Control />
                        </Form.Group>

                        <Form.Group as={Col} controlId="formGridState">
                            <Form.Label>State</Form.Label>
                            <Form.Control as="select" defaultValue="Choose...">
                                <option>Choose...</option>
                                <option>...</option>
                            </Form.Control>
                        </Form.Group>

                        <Form.Group as={Col} controlId="formGridZip">
                            <Form.Label>Zip</Form.Label>
                            <Form.Control />
                        </Form.Group>
                    </Form.Row>

                    <Form.Group id="formGridCheckbox">
                        <Form.Check type="checkbox" label="Check me out" />
                    </Form.Group>

                    <Button variant="primary" type="submit">Submit</Button>
                </Form>
            </Row>
        </>
    )
}

export interface RouterComponentMaterialProps extends RouteComponentProps<MaterialProps> { }
